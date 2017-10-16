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
* Author: TODO <Add proper author>
*
*/

#include <fstream>
#include <map>
#include <vector>

// dbg_plot
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

#include "Conversion.h"

namespace revolve
{
  namespace brain
  {
    std::map< CPPNConfigPtr, cppneat::GeneticEncodingPtr > RegisteredGenotypes;

    std::map< cppneat::Neuron::Ntype, cppneat::Neuron::NeuronTypeSpec >
            brain_spec;

    std::map< int, size_t > InputMap;

    std::map< int, size_t > OutputMap;

    void SetParameters(
            cppneat::Neuron::ParamSpec &_specification,
            const std::string &_name,
            const double _eps,
            const double _minimum,
            const double _maximum,
            const bool _minInclusive,
            const bool _maxInclusive)
    {
      _specification.name = _name;
      _specification.epsilon = _eps;
      _specification.maxInclusive = _maxInclusive;
      _specification.minInclusive = _minInclusive;
      _specification.min = _minimum;
      _specification.max = _maximum;
    }

    void SetBrainSpec(bool _isHyperNeat)
    {
      double eps = 1e-9;
      bool max_inclusive = false;
      bool min_inclusive = false;
      cppneat::Neuron::NeuronTypeSpec input;
      input.possibleLayers.push_back(cppneat::Neuron::INPUT_LAYER);

      brain_spec[cppneat::Neuron::INPUT] = input;

      cppneat::Neuron::NeuronTypeSpec sigmoid;
      cppneat::Neuron::ParamSpec bias_spec;
      SetParameters(bias_spec,
                    "rv:bias",
                    eps,
                    -1,
                    1,
                    max_inclusive,
                    min_inclusive);
      cppneat::Neuron::ParamSpec gain_spec;
      SetParameters(gain_spec,
                    "rv:gain",
                    eps,
                    0,
                    1,
                    max_inclusive,
                    min_inclusive);
      sigmoid.parameters.push_back(bias_spec);
      sigmoid.parameters.push_back(gain_spec);
      sigmoid.possibleLayers.push_back(cppneat::Neuron::HIDDEN_LAYER);
      sigmoid.possibleLayers.push_back(cppneat::Neuron::OUTPUT_LAYER);

      brain_spec[cppneat::Neuron::SIGMOID] = sigmoid;


      cppneat::Neuron::NeuronTypeSpec simple;
      simple.parameters.push_back(bias_spec);
      simple.parameters.push_back(gain_spec);
      simple.possibleLayers.push_back(cppneat::Neuron::HIDDEN_LAYER);
      simple.possibleLayers.push_back(cppneat::Neuron::OUTPUT_LAYER);

      brain_spec[cppneat::Neuron::SIMPLE] = simple;


      cppneat::Neuron::NeuronTypeSpec bias;
      bias.parameters.push_back(bias_spec);
      bias.possibleLayers.push_back(cppneat::Neuron::INPUT_LAYER);

      brain_spec[cppneat::Neuron::BIAS] = bias;

      if (not _isHyperNeat)
      {
        cppneat::Neuron::NeuronTypeSpec oscillator;
        cppneat::Neuron::ParamSpec period_spec;
        SetParameters(period_spec,
                      "rv:period",
                      eps,
                      0,
                      10,
                      max_inclusive,
                      min_inclusive);
        cppneat::Neuron::ParamSpec phase_offset_spec;
        SetParameters(phase_offset_spec,
                      "rv:phase_offset",
                      eps,
                      0,
                      3.14,
                      max_inclusive,
                      min_inclusive);
        cppneat::Neuron::ParamSpec amplitude_spec;
        SetParameters(amplitude_spec,
                      "rv:amplitude",
                      eps,
                      0,
                      10000,
                      max_inclusive,
                      min_inclusive);
        oscillator.parameters.push_back(period_spec);
        oscillator.parameters.push_back(phase_offset_spec);
        oscillator.parameters.push_back(amplitude_spec);
        oscillator.possibleLayers.push_back(cppneat::Neuron::INPUT_LAYER);

        brain_spec[cppneat::Neuron::OSCILLATOR] = oscillator;
      }
      else
      {
        cppneat::Neuron::NeuronTypeSpec input_oscillator;
        cppneat::Neuron::ParamSpec period_spec;
        SetParameters(period_spec,
                      "rv:period",
                      eps,
                      0,
                      10,
                      max_inclusive,
                      min_inclusive);
        cppneat::Neuron::ParamSpec phase_offset_spec;
        SetParameters(phase_offset_spec,
                      "rv:phase_offset",
                      eps,
                      0,
                      3.14,
                      max_inclusive,
                      min_inclusive);
        cppneat::Neuron::ParamSpec amplitude_spec;
        SetParameters(amplitude_spec,
                      "rv:amplitude",
                      eps,
                      0,
                      10000,
                      max_inclusive,
                      min_inclusive);
        input_oscillator.parameters.push_back(period_spec);
        input_oscillator.parameters.push_back(phase_offset_spec);
        input_oscillator.parameters.push_back(amplitude_spec);
        input_oscillator.possibleLayers.push_back(
                cppneat::Neuron::HIDDEN_LAYER);
        input_oscillator.possibleLayers.push_back(
                cppneat::Neuron::OUTPUT_LAYER);

        brain_spec[cppneat::Neuron::INPUT_OSCILLATOR] = input_oscillator;
      }

      if (not _isHyperNeat)
      {
        cppneat::Neuron::NeuronTypeSpec differential_neuron;
        differential_neuron.parameters.push_back(bias_spec);
        differential_neuron.possibleLayers.push_back(
                cppneat::Neuron::HIDDEN_LAYER);
        differential_neuron.possibleLayers.push_back(
                cppneat::Neuron::OUTPUT_LAYER);

        brain_spec[cppneat::Neuron::DIFFERENTIAL_CPG] = differential_neuron;
      }
    }


    CPPNConfigPtr convertForController(cppneat::GeneticEncodingPtr _genotype)
    {
      assert(not _genotype->isLayered_);
      std::vector< cppneat::NeuronGenePtr > neuron_genes =
              _genotype->neuronGenes_;
      std::vector< cppneat::ConnectionGenePtr > connection_genes =
              _genotype->connectionGenes_;

      std::map< int, NeuronPtr > innov_number_to_neuron;

      boost::shared_ptr< CPPNConfig > config(new CPPNConfig());
      for (cppneat::NeuronGenePtr neuron_gene : neuron_genes)
      {
        NeuronPtr newNeuron;
        std::string neuronId = neuron_gene->neuron_->neuronId_;
        std::map< std::string, double > neuron_params =
                neuron_gene->neuron_->parameters_;

        switch (neuron_gene->neuron_->layer_)
        {
          case cppneat::Neuron::INPUT_LAYER:
          {
            newNeuron.reset(new InputNeuron(neuronId, neuron_params));
            config->inputNeurons_.push_back(newNeuron);
            config->inputPositionMap_[newNeuron] =
                    InputMap[neuron_gene->InnovationNumber()];
            break;
          }
          case cppneat::Neuron::HIDDEN_LAYER:
          {
            switch (neuron_gene->neuron_->neuronType_)
            {
              case cppneat::Neuron::INPUT:
              case cppneat::Neuron::SIMPLE:
              {
                newNeuron.reset(new LinearNeuron(
                        neuronId,
                        neuron_params));
                break;
              }
              case cppneat::Neuron::SIGMOID:
              {
                newNeuron.reset(new SigmoidNeuron(
                        neuronId,
                        neuron_params));
                break;
              }
              case cppneat::Neuron::DIFFERENTIAL_CPG:
              {
                newNeuron.reset(new DifferentialCPG(
                        neuronId,
                        neuron_params));
                break;
              }
              case cppneat::Neuron::RYTHM_GENERATOR_CPG:
              {
                newNeuron.reset(new RythmGenerationCPG(
                        neuronId,
                        neuron_params));
                break;
              }
              case cppneat::Neuron::BIAS:
              {
                newNeuron.reset(new BiasNeuron(
                        neuronId,
                        neuron_params));
                break;
              }
              case cppneat::Neuron::OSCILLATOR:
              {
                newNeuron.reset(new OscillatorNeuron(
                        neuronId,
                        neuron_params));
                break;
              }
              case cppneat::Neuron::INPUT_OSCILLATOR:
              {
                newNeuron.reset(new InputDependentOscillatorNeuron(
                        neuronId,
                        neuron_params));
                break;
              }
              default:
              {
                throw std::runtime_error("Unkown neuron type to be converted");
              }
            }
            config->hiddenNeurons_.push_back(newNeuron);
            break;
          }
          case cppneat::Neuron::OUTPUT_LAYER:
          {
            switch (neuron_gene->neuron_->neuronType_)
            {
              case cppneat::Neuron::INPUT:
              case cppneat::Neuron::SIMPLE:
              {
                newNeuron.reset(new LinearNeuron(
                        neuronId,
                        neuron_params));
                break;
              }
              case cppneat::Neuron::SIGMOID:
              {
                newNeuron.reset(new SigmoidNeuron(
                        neuronId,
                        neuron_params));
                break;
              }
              case cppneat::Neuron::DIFFERENTIAL_CPG:
              {
                newNeuron.reset(new DifferentialCPG(
                        neuronId,
                        neuron_params));
                break;
              }
              case cppneat::Neuron::RYTHM_GENERATOR_CPG:
              {
                newNeuron.reset(new RythmGenerationCPG(
                        neuronId,
                        neuron_params));
                break;
              }
              case cppneat::Neuron::BIAS:
              {
                newNeuron.reset(new BiasNeuron(
                        neuronId,
                        neuron_params));
                break;
              }
              case cppneat::Neuron::OSCILLATOR:
              {
                newNeuron.reset(new OscillatorNeuron(
                        neuronId,
                        neuron_params));
                break;
              }
              case cppneat::Neuron::INPUT_OSCILLATOR:
              {
                newNeuron.reset(new InputDependentOscillatorNeuron(
                        neuronId,
                        neuron_params));
                break;
              }
              default:
              {
                throw std::runtime_error("Unknown neuron type to be converted");
              }
            }
            config->outputNeurons_.push_back(newNeuron);
            config->outputPositionMap_[newNeuron] =
                    OutputMap[neuron_gene->InnovationNumber()];
            break;
          }
          default:
          {
            throw std::runtime_error("Robot brain error");
          }
        }
        config->allNeurons_.push_back(newNeuron);
        config->idToNeuron_[neuronId] = newNeuron;
        innov_number_to_neuron[neuron_gene->InnovationNumber()] = newNeuron;
      }
      for (cppneat::ConnectionGenePtr connection_gene : connection_genes)
      {
        NeuronPtr dst = innov_number_to_neuron[connection_gene->to_];
        NeuralConnectionPtr newConnection(new NeuralConnection(
                innov_number_to_neuron[connection_gene->from_],
                dst,
                connection_gene->weight_));
        dst->AddIncomingConnection(
                dst->SocketId(),
                newConnection);
        config->connections_.push_back(newConnection);
      }
      RegisteredGenotypes[config] = _genotype;
      return config;
    }

    cppneat::GeneticEncodingPtr convertForLearner(CPPNConfigPtr _config)
    {
      return RegisteredGenotypes[_config];
    }

///////////////////////////////////////////////////////////////////////////////
/// RLPower_CPG~RLPower_CPPN
///////////////////////////////////////////////////////////////////////////////
    std::vector< double > convertPolicyToDouble(PolicyPtr _genotype)
    {
      return (*_genotype).at(0);
    }

    PolicyPtr convertDoubleToNull(std::vector< double > /*_phenotype*/)
    {
      return nullptr;  // input is not taken into account
    }
///////////////////////////////////////////////////////////////////////////////


    boost::shared_ptr< CPPNConfig > RafCpgNetwork;

    std::map< std::string, std::tuple< int, int, int>> neuron_coordinates;

    cppneat::GeneticEncodingPtr last_genotype_;

    boost::shared_ptr< LayeredExtNNConfig >
    convertForLayeredExtNN(cppneat::GeneticEncodingPtr genotype)
    {
      assert(genotype->isLayered_);
      std::vector< std::vector< cppneat::NeuronGenePtr>> layers =
              genotype->layers_;
      std::vector< cppneat::ConnectionGenePtr > connection_genes =
              genotype->connectionGenes_;

      std::map< int, NeuronPtr > neuron_inovation_numbers;

      boost::shared_ptr< LayeredExtNNConfig > cppn(new LayeredExtNNConfig());
      cppn->layers_ = std::vector< std::vector< NeuronPtr > >(
              layers.size(),
              std::vector< NeuronPtr >());

      for (size_t index = 0; index < layers.size(); ++index)
      {
        for (const auto neuron_gene : layers.at(index))
        {
          NeuronPtr new_neuron;
          std::string neuronId = neuron_gene->neuron_->neuronId_;
          std::map< std::string, double > neuron_params =
                  neuron_gene->neuron_->parameters_;

          switch (neuron_gene->neuron_->layer_)
          {
            case cppneat::Neuron::INPUT_LAYER:
            {
              new_neuron.reset(new InputNeuron(neuronId, neuron_params));
              cppn->layers_[index].push_back(new_neuron);
              cppn->inputPositionMap_[new_neuron] =
                      InputMap[neuron_gene->InnovationNumber()];
              break;
            }
            case cppneat::Neuron::HIDDEN_LAYER:
            {
              switch (neuron_gene->neuron_->neuronType_)
              {
                case cppneat::Neuron::INPUT:
                case cppneat::Neuron::SIMPLE:
                {
                  new_neuron.reset(new LinearNeuron(
                          neuronId,
                          neuron_params));
                  break;
                }
                case cppneat::Neuron::SIGMOID:
                {
                  new_neuron.reset(new SigmoidNeuron(
                          neuronId,
                          neuron_params));
                  break;
                }
                case cppneat::Neuron::DIFFERENTIAL_CPG:
                {
                  new_neuron.reset(new DifferentialCPG(
                          neuronId,
                          neuron_params));
                  break;
                }
                case cppneat::Neuron::RYTHM_GENERATOR_CPG:
                {
                  new_neuron.reset(new RythmGenerationCPG(
                          neuronId,
                          neuron_params));
                  break;
                }
                case cppneat::Neuron::BIAS:
                {
                  new_neuron.reset(new BiasNeuron(
                          neuronId,
                          neuron_params));
                  break;
                }
                case cppneat::Neuron::OSCILLATOR:
                {
                  new_neuron.reset(new OscillatorNeuron(
                          neuronId,
                          neuron_params));
                  break;
                }
                case cppneat::Neuron::INPUT_OSCILLATOR:
                {
                  new_neuron.reset(new InputDependentOscillatorNeuron(
                          neuronId,
                          neuron_params));
                  break;
                }
                default:
                {
                  throw std::runtime_error("Unkown neuron type converted");
                }
              }
              cppn->layers_[index].push_back(new_neuron);
              break;
            }
            case cppneat::Neuron::OUTPUT_LAYER:
            {
              switch (neuron_gene->neuron_->neuronType_)
              {
                case cppneat::Neuron::INPUT:
                case cppneat::Neuron::SIMPLE:
                {
                  new_neuron.reset(new LinearNeuron(
                          neuronId,
                          neuron_params));
                  break;
                }
                case cppneat::Neuron::SIGMOID:
                {
                  new_neuron.reset(new SigmoidNeuron(
                          neuronId,
                          neuron_params));
                  break;
                }
                case cppneat::Neuron::DIFFERENTIAL_CPG:
                {
                  new_neuron.reset(new DifferentialCPG(
                          neuronId,
                          neuron_params));
                  break;
                }
                case cppneat::Neuron::RYTHM_GENERATOR_CPG:
                {
                  new_neuron.reset(new RythmGenerationCPG(
                          neuronId,
                          neuron_params));
                  break;
                }
                case cppneat::Neuron::BIAS:
                {
                  new_neuron.reset(new BiasNeuron(
                          neuronId,
                          neuron_params));
                  break;
                }
                case cppneat::Neuron::OSCILLATOR:
                {
                  new_neuron.reset(new OscillatorNeuron(
                          neuronId,
                          neuron_params));
                  break;
                }
                case cppneat::Neuron::INPUT_OSCILLATOR:
                {
                  new_neuron.reset(new InputDependentOscillatorNeuron(
                          neuronId,
                          neuron_params));
                  break;
                }
                default:
                {
                  throw std::runtime_error("Unknown neuron type converted");
                }
              }
              cppn->layers_[index].push_back(new_neuron);
              cppn->outputPositionMap_[new_neuron] =
                      OutputMap[neuron_gene->InnovationNumber()];
              break;
            }
            default:
            {
              throw std::runtime_error("Robot brain error");
            }
          }
          cppn->idToNeuron_[neuronId] = new_neuron;
          neuron_inovation_numbers[neuron_gene->InnovationNumber()] =
                  new_neuron;
        }
      }
      for (cppneat::ConnectionGenePtr connection_gene : connection_genes)
      {
        auto destination_neuron =
                neuron_inovation_numbers[connection_gene->to_];
        NeuralConnectionPtr newConnection(new NeuralConnection(
                neuron_inovation_numbers[connection_gene->from_],
                destination_neuron,
                connection_gene->weight_));
        destination_neuron->AddIncomingConnection(
                destination_neuron->SocketId(),
                newConnection);
        cppn->connections_.push_back(newConnection);
      }
      return cppn;
    }

    void write_debugplot(
            boost::shared_ptr< CPPNConfig > conf,
            bool include_coordinates)
    {
      std::ofstream write_to("debug_plot_extnn.dot");
      boost::adjacency_list<> graph(conf->allNeurons_.size());
      for (size_t i = 0; i < conf->allNeurons_.size(); i++)
      {
        std::vector< std::pair< std::string, NeuralConnectionPtr>>
                connectionsToAdd =
                conf->allNeurons_[i]->IncomingConnections();

        for (const auto connectionToAdd : connectionsToAdd)
        {
          NeuronPtr input = connectionToAdd.second->GetInputNeuron();
          long indexInput = std::find(
                  conf->allNeurons_.begin(),
                  conf->allNeurons_.end(),
                  input) - conf->allNeurons_.begin();
          boost::add_edge(indexInput, i, graph);
        }
      }

      std::string *names = new std::string[conf->allNeurons_.size()];
      for (size_t i = 0; i < conf->allNeurons_.size(); i++)
      {
        std::stringstream nodeName;
        nodeName << conf->allNeurons_[i]->Id()
                 << " of type: " + conf->allNeurons_[i]->Type()
                 << std::endl;
        for (auto param : conf->allNeurons_[i]->Parameters())
        {
          nodeName << param.first << ": " << param.second << std::endl;
        }
        if (include_coordinates)
        {
          std::tuple< int, int, int >
                  coord = neuron_coordinates[conf->allNeurons_[i]->Id()];
          nodeName << "(x,y,z) = (" << std::get< 0 >(coord)
                   << "," << std::get< 1 >(coord)
                   << "," << std::get< 2 >(coord)
                   << ")" << std::endl;
        }
        names[i] = nodeName.str();
      }
      boost::write_graphviz(write_to, graph, boost::make_label_writer(names));
      delete[] names;
    }

//////////////////////////////////////////////////////////////////////////////
/// HyperNEAT_CPG
///////////////////////////////////////////////////////////////////////////////
    boost::shared_ptr< CPPNConfig > convertGEtoNN(
            cppneat::GeneticEncodingPtr _genotype)
    {
      auto cppn = convertForLayeredExtNN(_genotype);
      for (const auto connection : RafCpgNetwork->connections_)
      {
        auto src_neuron = connection->GetInputNeuron();
        auto dst_neuron = connection->GetOutputNeuron();
        auto coord_src = neuron_coordinates[src_neuron->Id()];
        auto coord_dst = neuron_coordinates[dst_neuron->Id()];
        for (const auto neuron : cppn->layers_.at(0))
        {
          // could be faster by neuron->Id()[6] but less easy to read
          if (neuron->Id() == "Input-0")
          {
            neuron->SetInput(std::get< 0 >(coord_src));
          }
          else if (neuron->Id() == "Input-1")
          {
            neuron->SetInput(std::get< 1 >(coord_src));
          }
          else if (neuron->Id() == "Input-2")
          {
            neuron->SetInput(std::get< 2 >(coord_src));
          }
          else if (neuron->Id() == "Input-3")
          {
            neuron->SetInput(std::get< 0 >(coord_dst));
          }
          else if (neuron->Id() == "Input-4")
          {
            neuron->SetInput(std::get< 1 >(coord_dst));
          }
          else if (neuron->Id() == "Input-5")
          {
            neuron->SetInput(std::get< 2 >(coord_dst));
          }
        }

        for (const auto layer : cppn->layers_)
        {
          for (const auto neuron : layer)
          {
            neuron->Update(0);
          }
          for (const auto neuron : layer)
          {
            neuron->FlipState();
          }
        }

        for (const auto outNeuron : cppn->layers_.at(cppn->layers_.size() - 1))
        {
          if (outNeuron->Id() == "weight")
          {
            connection->SetWeight(outNeuron->Output());
            break;
          }
        }
      }
      for (const auto neuron : RafCpgNetwork->allNeurons_)
      {
        // Retrieve coordinates of source and destination neuron
        std::tuple< int, int, int >
                coord_src = neuron_coordinates[neuron->Id()];
        std::tuple< int, int, int > coord_dst = std::make_tuple(0, 0, 0);
        for (const auto input_neuron : cppn->layers_[0])
        {
          // could be faster by input_neuron->Id()[6] but less easy to read
          if (input_neuron->Id() == "Input-0")
          {
            input_neuron->SetInput(std::get< 0 >(coord_src));
          }
          else if (input_neuron->Id() == "Input-1")
          {
            input_neuron->SetInput(std::get< 1 >(coord_src));
          }
          else if (input_neuron->Id() == "Input-2")
          {
            input_neuron->SetInput(std::get< 2 >(coord_src));
          }
          else if (input_neuron->Id() == "Input-3")
          {
            input_neuron->SetInput(std::get< 0 >(coord_dst));
          }
          else if (input_neuron->Id() == "Input-4")
          {
            input_neuron->SetInput(std::get< 1 >(coord_dst));
          }
          else if (input_neuron->Id() == "Input-5")
          {
            input_neuron->SetInput(std::get< 2 >(coord_dst));
          }
        }

        // Feed CPPN with input coordinates
        for (std::vector< NeuronPtr > layer : cppn->layers_)
        {
          for (NeuronPtr hidden_neuron : layer)
          {
            hidden_neuron->Update(0);
          }
          for (NeuronPtr hidden_neuron : layer)
          {
            hidden_neuron->FlipState();
          }
        }

        std::map< std::string, double > params;
        for (auto outNeuron : cppn->layers_.at(cppn->layers_.size() - 1))
        {
          params[outNeuron->Id()] = outNeuron->Output();
        }
        neuron->SetParameters(params);
      }
      last_genotype_ = _genotype;
      // write_debugplot(RafCpgNetwork, true);
      return RafCpgNetwork;
    }

    cppneat::GeneticEncodingPtr convertNNtoGE(
            CPPNConfigPtr /*config*/)
    {
      return last_genotype_;
    }
///////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
/// HyperNEAT_Splines
///////////////////////////////////////////////////////////////////////////////
    std::vector< std::pair< int, int>> sorted_coordinates;

    PolicyPtr policy;

    size_t spline_size;

    size_t update_rate;

    size_t cur_step = 0;

    cppneat::GeneticEncodingPtr HyperNeatSplines()
    {
      int innov_number = 1;
      cppneat::GeneticEncodingPtr ret(new cppneat::GeneticEncoding(true));

      // Add input layer (x, y, z) for splines
      std::map< std::string, double > initial_neuron_params;
      for (size_t i = 0; i < 3; i++)
      {
        // better names (like input x1 etc) might help
        cppneat::NeuronPtr neuron(new cppneat::Neuron(
                "Input-" + std::to_string(i),
                cppneat::Neuron::INPUT_LAYER,
                cppneat::Neuron::INPUT,
                initial_neuron_params));

        // Increment innovation number
        cppneat::NeuronGenePtr neuron_gene(new cppneat::NeuronGene(
                neuron,
                innov_number++,
                true));
        ret->AddNeuron(neuron_gene, 0, i == 0);
      }

      // Add output layer
      initial_neuron_params["rv:bias"] = 0;
      initial_neuron_params["rv:gain"] = 0;
      cppneat::NeuronPtr weight_neuron(new cppneat::Neuron(
              "weight",
              cppneat::Neuron::OUTPUT_LAYER,
              cppneat::Neuron::SIMPLE,
              initial_neuron_params));
      cppneat::NeuronGenePtr weight_neuron_gene(new cppneat::NeuronGene(
              weight_neuron,
              innov_number++,
              true));
      ret->AddNeuron(weight_neuron_gene, 1, true);

      // Connect every input with every output
      for (size_t i = 0; i < 3; ++i)
      {
        cppneat::ConnectionGenePtr
                connection_to_weight(new cppneat::ConnectionGene(
                weight_neuron_gene->InnovationNumber(),
                i + 1,
                0,
                innov_number++,
                true,
                ""));
        ret->AddConnection(connection_to_weight);
      }
      return ret;
    }

    PolicyPtr convertForSplinesFromHyper(cppneat::GeneticEncodingPtr _genotype)
    {
      // TODO: fix update rate
      boost::shared_ptr< LayeredExtNNConfig > cppn =
              convertForLayeredExtNN(_genotype);
      if (++cur_step >= update_rate)
      {
        spline_size++;
        cur_step = 0;
      }
      policy = PolicyPtr(new Policy(
              sorted_coordinates.size(),
              Spline(spline_size, 0)));
      for (size_t j = 0; j < sorted_coordinates.size(); ++j)
      {
        for (size_t i = 0; i < spline_size; ++i)
        {
          std::tuple< double, double, double > coord(
                  sorted_coordinates[j].first,
                  sorted_coordinates[j].second,
                  i / (static_cast<double >(spline_size)));
          for (const auto neuron : cppn->layers_.at(0))
          {
            // could be faster by neuron->Id()[6] but less easy to read
            if (neuron->Id() == "Input-0")
            {
              neuron->SetInput(std::get< 0 >(coord));
            }
            else if (neuron->Id() == "Input-1")
            {
              neuron->SetInput(std::get< 1 >(coord));
            }
            else if (neuron->Id() == "Input-2")
            {
              neuron->SetInput(std::get< 2 >(coord));
            }
          }

          for (const auto layer : cppn->layers_)
          {
            for (const auto neuron : layer)
            {
              neuron->Update(0);
            }
            for (const auto neuron : layer)
            {
              neuron->FlipState();
            }
          }

          // Retrieve the output value
          for (const auto outNeuron : cppn->layers_[cppn->layers_.size() - 1])
          {
            if (outNeuron->Id() == "weight")
            {
              (*policy)[j][i] = outNeuron->Output();
              break;
            }
          }
        }
      }
      last_genotype_ = _genotype;
      return policy;
    }

    cppneat::GeneticEncodingPtr
    convertForHyperFromSplines(PolicyPtr /*policy*/)
    {
      return last_genotype_;
    }
  }
}
