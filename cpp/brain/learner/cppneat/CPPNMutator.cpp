/*
* Copyright (C) 2017 Vrije Universiteit Amsterdam
*
* Licensed under the Apache License, Version 2.0 (the "License");
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Description: TODO: <Add brief description about file purpose>
* Author: Rafael Kiesel
*
*/

#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "CPPNMutator.h"

namespace cppneat
{
  std::vector< Neuron::Ntype > Mutator::AddableTypes(
          std::map< Neuron::Ntype,
                    Neuron::NeuronTypeSpec > _specification)
  {
    std::vector< Neuron::Ntype > possible;
    for (auto it : _specification)
    {
      Neuron::NeuronTypeSpec spec = it.second;
      if (std::find(spec.possibleLayers.begin(),
                    spec.possibleLayers.end(),
                    Neuron::HIDDEN_LAYER) not_eq spec.possibleLayers.end())
      {
        possible.push_back(it.first);
      }
    }
    return possible;
  }

  Mutator::Mutator(
            std::map< Neuron::Ntype, Neuron::NeuronTypeSpec > brain_spec,
            double new_connection_sigma,
            int _innovationNumber,
            int _maxAttempts,
            std::vector< Neuron::Ntype > addable_neurons)
          : specification_(brain_spec)
          , tauConnectionSigma_(new_connection_sigma)
          , innovationNumber_(_innovationNumber)
          , maxAttempts_(_maxAttempts)
          , addableNeurons_(addable_neurons)
  {
    std::random_device rd;
    generator_.seed(rd());
    if (addable_neurons.size() == 0)
    {
      this->addableNeurons_ = AddableTypes(brain_spec);
    }
  }

  void Mutator::RegisteStartingGenotype(GeneticEncodingPtr _genotype)
  {
    for (ConnectionGenePtr connection : _genotype->connectionGenes_)
    {
      std::pair< int, int > innovation(connection->from_,
                                       connection->to_);
      connectionInnovations_[innovation] = connection->InnovationNumber();
    }
  }

  void Mutator::RecordInnovations(const std::string _yamlPath)
  {
    std::ofstream outputFile;
    outputFile.open(_yamlPath, std::ios::out | std::ios::trunc);
    outputFile << "- in_no: " << innovationNumber_ << std::endl;
    outputFile << "- connection_innovations: " << std::endl;
    for (auto connectio : connectionInnovations_)
    {
      outputFile << "  - connection_innovation: " << std::endl;
      outputFile << "      mark_from: "
                 << connectio.first.first << std::endl;
      outputFile << "      mark_to: "
                 << connectio.first.second << std::endl;
      outputFile << "      in_no: "
                 << connectio.second << std::endl;
    }

    outputFile << "- neuron_innovations: " << std::endl;
    for (auto neuron : neuronInnovations_)
    {
      outputFile << "  - neuron_innovation: " << std::endl;
      outputFile << "      conn_split: "
                 << neuron.first.first << std::endl;
      outputFile << "      ntype: "
                 << neuron.first.second << std::endl;
      outputFile << "      in_nos: " << std::endl;
      for (int in_no : neuron.second)
      {
        outputFile << "      - in_no: " << in_no << std::endl;
      }
    }
    outputFile.close();
  }

  void Mutator::InsertConnectionInnovation(
          const size_t _from,
          const size_t _to,
          const size_t _innovationNumber)
  {
    this->connectionInnovations_.insert(
            {{_from, _to},
             _innovationNumber});
    if (this->innovationNumber_ < _innovationNumber)
    {
      this->innovationNumber_ = _innovationNumber;
    }
  }

  void Mutator::InsertNeuronInnovation(
          const Neuron::Ntype _neuronType,
          const size_t _innovationNumber)
  {
    for (auto connection : connectionInnovations_)
    {
      if (_innovationNumber == connection.first.second)
      {
        std::vector< int > innovations(_innovationNumber);
        this->neuronInnovations_.insert(
                {{connection.second, _neuronType},
                 innovations});
      }
    }
    if (this->innovationNumber_ < _innovationNumber)
    {
      this->innovationNumber_ = _innovationNumber;
    }
  }

  void Mutator::LoadRegisteredInnovations(const std::string _yamlPath)
  {
    std::ifstream outputFile(_yamlPath);
    if (not outputFile.good())
    {
      return;
    }
    outputFile.close();
    YAML::Node yamlFile = YAML::LoadFile(_yamlPath);
    if (yamlFile.IsNull())
    {
      std::cout << "Failed to load the yaml file."
              "If this is the first run do not worry." << std::endl;
      return;
    }
    innovationNumber_ = yamlFile[0]["in_no"].as< int >();
    connectionInnovations_.clear();
    for (size_t i = 0; i < yamlFile[1]["connection_innovations"].size(); i++)
    {
      YAML::Node connection_innovation =
              yamlFile[1]["connection_innovations"][i][
                      "connection_innovation"];
      int mark_from = connection_innovation["mark_from"].as< int >();
      int mark_to = connection_innovation["mark_to"].as< int >();
      int in_no = connection_innovation["in_no"].as< int >();
      connectionInnovations_.insert({{mark_from, mark_to}, in_no});
    }
    neuronInnovations_.clear();
    for (size_t i = 0; i < yamlFile[2]["neuron_innovations"].size(); i++)
    {
      YAML::Node neuron_innovation =
              yamlFile[2]["neuron_innovations"][i]["neuron_innovation"];
      int conn_split = neuron_innovation["conn_split"].as< int >();
      Neuron::Ntype ntype =
              static_cast<Neuron::Ntype>(neuron_innovation["ntype"].as< int >());
      std::vector< int > in_nos;
      for (size_t j = 0; j < neuron_innovation["in_nos"].size(); j++)
      {
        in_nos.push_back(neuron_innovation["in_nos"][j]["in_no"].as< int >());
      }
      neuronInnovations_.insert({{conn_split, ntype}, in_nos});
    }
  }

  void Mutator::MutateNeuronParams(
          GeneticEncodingPtr _genotype,
          const double _probability,
          const double _sigma)
  {
    std::uniform_real_distribution< double > uniform(0, 1);
    if (not _genotype->isLayered_)
    {
      for (NeuronGenePtr neuron_gene : _genotype->neuronGenes_)
      {
        if (uniform(generator_) < _probability)
        {
          std::vector< Neuron::ParamSpec > neuron_params =
                  specification_[neuron_gene->neuron_->neuronType_].parameters;
          if (neuron_params.size() > 0)
          {
            std::uniform_int_distribution< int >
                    uniform_int(0, neuron_params.size() - 1);
            Neuron::ParamSpec param = neuron_params[uniform_int(generator_)];
            double cur_val = neuron_gene->neuron_->parameters_[param.name];
            //          std::normal_distribution<double>
            // normal(0,sigma*(param.max_value-param.min_value));
            std::normal_distribution< double > normal(0, _sigma);
            cur_val += normal(generator_);
            neuron_gene->neuron_->SetNeuronParameters(cur_val, param);
          }
        }
      }
    }
    else
    {
      for (auto layer : _genotype->layers_)
      {
        for (NeuronGenePtr neuron_gene : layer)
        {
          if (uniform(generator_) < _probability)
          {
            std::vector< Neuron::ParamSpec > neuron_params =
                    specification_[neuron_gene->neuron_->neuronType_].parameters;
            if (neuron_params.size() > 0)
            {
              std::uniform_int_distribution< int >
                      uniform_int(0, neuron_params.size() - 1);
              Neuron::ParamSpec param = neuron_params[uniform_int(generator_)];
              double cur_val = neuron_gene->neuron_->parameters_[param.name];
              //            std::normal_distribution<double>
              // normal(0,sigma*(param.max_value-param.min_value));
              std::normal_distribution< double > normal(0, _sigma);
              cur_val += normal(generator_);
              neuron_gene->neuron_->SetNeuronParameters(cur_val, param);
            }
          }
        }
      }
    }
  }

  void Mutator::MutateWeights(
          GeneticEncodingPtr genotype,
          const double _probability,
          const double _sigma)
  {
    std::uniform_real_distribution< double > uniform(0, 1);
    std::normal_distribution< double > normal(0, _sigma);
    for (ConnectionGenePtr connection_gene : genotype->connectionGenes_)
    {
      if (uniform(generator_) < _probability)
      {
        connection_gene->weight_ += normal(generator_);
      }
    }
  }

  void Mutator::MutateStructure(
          GeneticEncodingPtr _genotype,
          const double _probability)
  {
    std::uniform_real_distribution< double > uniform(0, 1);
    if (uniform(generator_) < _probability)
    {
      if (_genotype->NumConnections() == 0)
      {
        AddConnectionMutation(_genotype, tauConnectionSigma_);
      }
      else
      {
        if (uniform(generator_) < 0.5)
        {
          AddConnectionMutation(_genotype, tauConnectionSigma_);
        }
        else
        {
          AddNeuronMutation(_genotype, tauConnectionSigma_);
        }
      }
    }
  }

  bool Mutator::AddConnectionMutation(
          GeneticEncodingPtr _genotype,
          const double _sigma)
  {
    size_t numAttempts = 1;
    if (not _genotype->isLayered_)
    {
      std::uniform_int_distribution< int >
              choice(0, _genotype->NumLayers() - 1);
      NeuronGenePtr fromNeuron = _genotype->neuronGenes_[choice(generator_)];
      NeuronGenePtr toNeuron = _genotype->neuronGenes_[choice(generator_)];
      size_t from = fromNeuron->InnovationNumber();
      size_t to = toNeuron->InnovationNumber();

      while (_genotype->ExistingConnection(from, to)
             or toNeuron->neuron_->layer_ == Neuron::INPUT_LAYER)
      {
        fromNeuron = _genotype->neuronGenes_[choice(generator_)];
        toNeuron = _genotype->neuronGenes_[choice(generator_)];
        from = fromNeuron->InnovationNumber();
        to = toNeuron->InnovationNumber();

        ++numAttempts;
        if (numAttempts > this->maxAttempts_)
        {
          return false;
        }
      }
      std::normal_distribution< double > normal(0, _sigma);
      this->AddConnection(from, to, normal(generator_), _genotype, "");
#ifdef CPPNEAT_DEBUG
      if (not genotype->is_valid())
      {
          std::cerr << "add connection mutation caused invalid genotye"
          << std::endl;
          throw std::runtime_error("mutation error");
      }
#endif
    }
    else
    {
      std::uniform_int_distribution< size_t >
              choice(0, _genotype->NumNeurons() - 1);
      std::pair< size_t, size_t > indexFrom =
              _genotype->CoordinatesByIndex(choice(generator_));
      std::pair< size_t, size_t > indexTo =
              _genotype->CoordinatesByIndex(choice(generator_));
      NeuronGenePtr fromNeuron =
              _genotype->layers_[indexFrom.first][indexFrom.second];
      NeuronGenePtr toNeuron =
              _genotype->layers_[indexTo.first][indexTo.second];
      size_t from = fromNeuron->InnovationNumber();
      size_t to = toNeuron->InnovationNumber();

      while (_genotype->ExistingConnection(from, to)
             or indexFrom.first >= indexTo.first)
      {
        indexFrom = _genotype->CoordinatesByIndex(choice(generator_));
        indexTo = _genotype->CoordinatesByIndex(choice(generator_));
        fromNeuron = _genotype->layers_[indexFrom.first][indexFrom.second];
        toNeuron = _genotype->layers_[indexTo.first][indexTo.second];
        from = fromNeuron->InnovationNumber();
        to = toNeuron->InnovationNumber();

        ++numAttempts;
        if (numAttempts > this->maxAttempts_)
        {
          return false;
        }
      }
      std::normal_distribution< double > normal(0, _sigma);
      AddConnection(from, to, normal(generator_), _genotype, "");
#ifdef CPPNEAT_DEBUG
      if (not genotype->is_valid())
      {
          std::cerr << "add connection mutation caused invalid genotye"
           << std::endl;
          throw std::runtime_error("mutation error");
      }
#endif
    }
    return true;
  }

  std::map< std::string, double > RandomParameters(
          Neuron::NeuronTypeSpec _specification,
          const double _sigma)
  {
    std::map< std::string, double > params;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::normal_distribution< double > normal(0, _sigma);
    for (auto spec : _specification.parameters)
    {
      params[spec.name] = normal(generator);
    }
    return params;
  }

  void Mutator::AddNeuronMutation(
          GeneticEncodingPtr _genotype,
          const double _sigma)
  {
    assert(_genotype->NumConnections() > 0);
    assert(addableNeurons_.size() > 0);
    if (not _genotype->isLayered_)
    {
      std::uniform_int_distribution< int >
              choice1(0, _genotype->NumConnections() - 1);
      int splitId = choice1(generator_);
      ConnectionGenePtr split = _genotype->connectionGenes_[splitId];

      double oldWeight = split->weight_;
      size_t from = split->from_;
      size_t to = split->to_;

      _genotype->RemoveConnection(splitId);
      NeuronPtr fromNeuron = boost::dynamic_pointer_cast< NeuronGene >(
                      _genotype->Find(from))->neuron_;
      NeuronPtr toNeuron = boost::dynamic_pointer_cast< NeuronGene >(
                      _genotype->Find(to))->neuron_;

      std::uniform_int_distribution< int >
              choice2(0, addableNeurons_.size() - 1);
      Neuron::Ntype neuronType = addableNeurons_[choice2(generator_)];

      std::map< std::string, double > neuronParameters = RandomParameters(
              specification_[neuronType],
              _sigma);

      NeuronPtr neuron_middle(new Neuron(
              ("augment" + std::to_string(innovationNumber_ + 1)),
              Neuron::HIDDEN_LAYER,
              neuronType,
              neuronParameters));
      int mark_middle = AddNeuron(neuron_middle, _genotype, split);
      AddConnection(from, mark_middle, oldWeight, _genotype, "");
      AddConnection(mark_middle, to, 1.0, _genotype, "");
    }
    else
    {
      std::uniform_int_distribution< int >
              choice1(0, _genotype->NumConnections() - 1);
      int splitId = choice1(generator_);
      ConnectionGenePtr split = _genotype->connectionGenes_[splitId];

      double old_weight = split->weight_;
      size_t from = split->from_;
      size_t to = split->to_;

      _genotype->RemoveConnection(splitId);
      NeuronPtr fromNeuron =
              boost::dynamic_pointer_cast< NeuronGene >(
                      _genotype->Find(from))->neuron_;
      NeuronPtr toNeuron =
              boost::dynamic_pointer_cast< NeuronGene >(
                      _genotype->Find(to))->neuron_;

      std::uniform_int_distribution< int >
              choice2(0, addableNeurons_.size() - 1);
      Neuron::Ntype new_neuron_type = addableNeurons_[choice2(generator_)];

      std::map< std::string, double > new_neuron_params =
              RandomParameters(specification_[new_neuron_type], _sigma);

      NeuronPtr neuron_middle(new Neuron(
              "augment" + std::to_string(innovationNumber_ + 1),
              Neuron::HIDDEN_LAYER,
              new_neuron_type,
              new_neuron_params));
      int mark_middle = AddNeuron(neuron_middle, _genotype, split);
#ifdef CPPNEAT_DEBUG
      if (not genotype->is_valid())
      {
          std::cerr << "add neuron mutation caused invalid genotye1"
          << std::endl;
          throw std::runtime_error("mutation error");
      }
#endif
      AddConnection(from, mark_middle, old_weight, _genotype, "");
#ifdef CPPNEAT_DEBUG
      if (not genotype->is_valid())
      {
          std::cerr << "add neuron mutation caused invalid genotye2"
          << std::endl;
          throw std::runtime_error("mutation error");
      }
#endif
      AddConnection(mark_middle, to, 1.0, _genotype, "");
    }
#ifdef CPPNEAT_DEBUG
    if (not genotype->is_valid())
    {
        std::cerr << "add neuron mutation caused invalid genotye" << std::endl;
        throw std::runtime_error("mutation error");
    }
#endif
  }

  void Mutator::RemoveConnectionMutation(GeneticEncodingPtr _genotype)
  {
    if (_genotype->NumConnections() == 0)
    {
      return;
    }
    std::uniform_int_distribution< int >
            choice(0, _genotype->NumConnections() - 1);
    _genotype->connectionGenes_.erase(
            _genotype->connectionGenes_.begin() + choice(generator_));
  }

  void Mutator::RemoveNeuronMutation(GeneticEncodingPtr _genotype)
  {
    std::vector< int > hidden_neuron_ids;
    for (size_t i = 0; i < _genotype->NumLayers(); i++)
    {
      if (_genotype->neuronGenes_[i]->neuron_->layer_ == Neuron::HIDDEN_LAYER)
      {
        hidden_neuron_ids.push_back(i);
      }
    }
    std::uniform_int_distribution< int >
            choice(0, hidden_neuron_ids.size() - 1);
    int geneId = hidden_neuron_ids[choice(generator_)];
    NeuronGenePtr neuron_gene = _genotype->neuronGenes_[geneId];
    size_t neuron_mark = neuron_gene->InnovationNumber();

    std::vector< int > bad_connections;
    for (size_t i = 0; i < _genotype->NumConnections(); i++)
    {
      if (_genotype->connectionGenes_[i]->from_ == neuron_mark
          or _genotype->connectionGenes_[i]->to_ == neuron_mark)
      {
        bad_connections.push_back(i);
      }
    }
    for (int i = bad_connections.size() - 1; i >= 0; i--)
    {
      _genotype->RemoveConnection(bad_connections[i]);
    }
    _genotype->RemoveNeuron(geneId);
  }

  int Mutator::AddNeuron(
          NeuronPtr _neuron,
          GeneticEncodingPtr _genotype,
          ConnectionGenePtr _split)
  {
    int connection_split_in = _split->InnovationNumber();
    std::pair< int, Neuron::Ntype > neuron_pair(
            connection_split_in,
            _neuron->neuronType_);
    if (neuronInnovations_.find(neuron_pair) not_eq neuronInnovations_.end())
    {
      size_t i = 0;
      while (i < neuronInnovations_[neuron_pair].size()
             and _genotype->Find(neuronInnovations_[neuron_pair][i])
                not_eq nullptr)
      {
        i++;
      }
      // some previous innovation is not are already present in
      // the genome-> add a it
      if (i < neuronInnovations_[neuron_pair].size())
      {
        NeuronGenePtr new_neuron_gene(new NeuronGene(
                _neuron,
                neuronInnovations_[neuron_pair][i],
                true,
                "none",
                -1));
        if (not _genotype->isLayered_)
        {
          _genotype->AddNeuron(new_neuron_gene);
        }
        else
        {
          size_t from = _split->from_;
          size_t to = _split->to_;
          std::pair< size_t, size_t > fromIndex =
                  _genotype->CoordinatesByInnovationNumber(from);
          std::pair< size_t, size_t > toIndex =
                  _genotype->CoordinatesByInnovationNumber(to);
          assert(fromIndex.first < toIndex.first);

          // we need a new layer iff the two layers are
          // "right next to each other"
          _genotype->AddNeuron(
                  new_neuron_gene,
                  fromIndex.first + 1,
                  fromIndex.first + 1 ==
                  toIndex.first);
        }
        return new_neuron_gene->InnovationNumber();
      }
    }
    // new innovation -> add new neuron with new innovation number
    NeuronGenePtr new_neuron_gene(new NeuronGene(
            _neuron,
            ++innovationNumber_,
            true,
            "none",
            -1));
    // in base case a new vector is constructed here
    neuronInnovations_[neuron_pair].push_back(innovationNumber_);
    if (not _genotype->isLayered_)
    {
      _genotype->AddNeuron(new_neuron_gene);
    }
    else
    {
      size_t from = _split->from_;
      size_t to = _split->to_;
      std::pair< size_t, size_t > fromIndex =
              _genotype->CoordinatesByInnovationNumber(from);
      std::pair< size_t, size_t > toIndex =
              _genotype->CoordinatesByInnovationNumber(to);
      assert(fromIndex.first < toIndex.first);

      // we need a new layer iff the two layers are "right next to each other"
      _genotype->AddNeuron(
              new_neuron_gene,
              fromIndex.first + 1,
              fromIndex.first + 1 == toIndex.first);
    }
    return new_neuron_gene->InnovationNumber();
  }

  int Mutator::AddConnection(
          const size_t _from,
          const size_t _to,
          const double _weight,
          GeneticEncodingPtr _genotype,
          const std::string _socket)
  {
    std::pair< int, int > innovation_pair(_from, _to);
    if (connectionInnovations_.find(innovation_pair)
        not_eq connectionInnovations_.end())
    {
      GenePtr found = _genotype->Find(
                      connectionInnovations_[innovation_pair]);
      if (found != nullptr)
      {
        boost::dynamic_pointer_cast< ConnectionGene >(found)->SetEnabled(true);
        return connectionInnovations_[innovation_pair];
      }
      else
      {
        ConnectionGenePtr new_conn_gene(new ConnectionGene(
                _to,
                _from,
                _weight,
                connectionInnovations_[innovation_pair],
                true,
                "none",
                -1,
                _socket));
        _genotype->AddConnection(new_conn_gene);
        return new_conn_gene->InnovationNumber();
      }
    }
    ConnectionGenePtr new_conn_gene(new ConnectionGene(
            _to,
            _from,
            _weight,
            ++innovationNumber_,
            true,
            "none",
            -1,
            _socket));
    connectionInnovations_[innovation_pair] = innovationNumber_;
    _genotype->AddConnection(new_conn_gene);
    return new_conn_gene->InnovationNumber();
  }
}
