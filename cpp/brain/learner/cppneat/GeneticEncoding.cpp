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

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>

#include "GeneticEncoding.h"

namespace cppneat
{
  bool CompareGenes(
          const GenePtr _gene1,
          const GenePtr _gene2)
  {
    return _gene1->InnovationNumber() < _gene2->InnovationNumber();
  }

  // non-layered
  void GeneticEncoding::AddNeuron(NeuronGenePtr _neuron)
  {
    this->SortedGenes();
    this->neuronGenes_.push_back(_neuron);
    auto ins = std::upper_bound(
            this->allSortedGenes_.begin(),
            this->allSortedGenes_.end(),
            boost::dynamic_pointer_cast< Gene >(_neuron),
            CompareGenes);
    this->allSortedGenes_.insert(
            ins,
            boost::dynamic_pointer_cast< Gene >(_neuron));
  }

  // layered
  void GeneticEncoding::AddNeuron(
          NeuronGenePtr _neuron,
          const size_t _layer,
          const bool _newLayer)
  {
    this->SortedGenes();
    if (_newLayer)
    {
      layers_.emplace(
              this->layers_.begin() + _layer,
              std::vector< NeuronGenePtr >(1, _neuron));
    }
    else
    {
      this->layers_[_layer].push_back(_neuron);
    }
    auto ins = std::upper_bound(
            this->allSortedGenes_.begin(),
            this->allSortedGenes_.end(),
            boost::dynamic_pointer_cast< Gene >(_neuron),
            CompareGenes);
    this->allSortedGenes_.insert(
            ins,
            boost::dynamic_pointer_cast< Gene >(_neuron));
  }


  void GeneticEncoding::AddConnection(ConnectionGenePtr _connection)
  {
    this->SortedGenes();
    this->connectionGenes_.push_back(_connection);
    auto ins = std::upper_bound(
            this->allSortedGenes_.begin(),
            this->allSortedGenes_.end(),
            boost::dynamic_pointer_cast< Gene >(_connection),
            CompareGenes);
    this->allSortedGenes_.insert(
            ins,
            boost::dynamic_pointer_cast< Gene >(_connection));
  }

  // ALERT::only works non-layered but seems to be not needed
  void GeneticEncoding::Adopt(GeneticEncodingPtr adoptee)
  {
    SortedGenes();
    std::vector< GenePtr > adoptee_sorted_genes = adoptee->SortedGenes();
    std::vector< std::pair< GenePtr, GenePtr>>
            pairs = GeneticEncoding::Pair(
            adoptee_sorted_genes,
            allSortedGenes_);
    for (std::pair< GenePtr, GenePtr > pair : pairs)
    {
      // adopt genes that i dont have
      if (pair.first != nullptr && pair.second == nullptr)
      {
        if (pair.first->type_ == Gene::NEURON_GENE)
        {
          AddNeuron(
                  boost::dynamic_pointer_cast< NeuronGene >(pair.first));
        }
        else if (pair.first->type_ == Gene::CONNECTION_GENE)
        {
          AddConnection(
                  boost::dynamic_pointer_cast< ConnectionGene >(pair.first));
        }
      }
    }
  }

  std::pair< size_t, size_t > GeneticEncoding::CoordinatesByIndex(
          const size_t _index)
  {
    size_t layer = 0;
    size_t in_layer = 0;
    for (size_t i = 0; i < _index; ++i)
    {
      if (layers_.at(layer).size() == in_layer + 1)
      {
        layer++;
        in_layer = 0;
      }
      else
      {
        in_layer++;
      }
    }
    return {layer, in_layer};
  }

  std::pair< size_t, size_t > GeneticEncoding::CoordinatesByInnovationNumber(
          const size_t _innovationNumber)
  {
    size_t layer = 0, in_layer = 0;
    while (this->layers_.at(layer).at(in_layer)->InnovationNumber() not_eq
                                                          _innovationNumber)
    {
      assert(this->layers_.at(layer) == nullptr && "A is not equal to B");
      if (this->layers_.at(layer).size() == in_layer + 1)
      {
        layer++;
        in_layer = 0;
      }
      else
      {
        in_layer++;
      }
    }
    return {layer, in_layer};
  }

  GeneticEncodingPtr GeneticEncoding::Copy()
  {
    if (not this->isLayered_)
    {
      GeneticEncodingPtr copy_gen(new GeneticEncoding(false));
      for (const auto neuron_gene : neuronGenes_)
      {
        NeuronGenePtr copy_neuron(new NeuronGene(*neuron_gene));
        copy_gen->AddNeuron(copy_neuron);
      }
      for (const auto connection_gene : connectionGenes_)
      {
        ConnectionGenePtr copy_conn(new ConnectionGene(*connection_gene));
        copy_gen->AddConnection(copy_conn);
      }
      return copy_gen;
    }
    else
    {
      GeneticEncodingPtr copy_gen(new GeneticEncoding(true));
      for (size_t i = 0; i < layers_.size(); i++)
      {
        bool first = true;
        for (const auto neuron_gene : layers_[i])
        {
          if (first)
          {
            NeuronGenePtr copy_neuron(new NeuronGene(*neuron_gene));
            copy_gen->AddNeuron(copy_neuron, i, true);
            first = false;
          }
          else
          {
            NeuronGenePtr copy_neuron(new NeuronGene(*neuron_gene));
            copy_gen->AddNeuron(copy_neuron, i, false);
          }
        }
      }
      for (const auto connection_gene : connectionGenes_)
      {
        ConnectionGenePtr copy_conn(new ConnectionGene(*connection_gene));
        copy_gen->AddConnection(copy_conn);
      }
      return copy_gen;
    }
  }

  double GeneticEncoding::Dissimilarity(
          GeneticEncodingPtr _genotype1,
          GeneticEncodingPtr _genotype2,
          const double _excess_coef,
          const double _disjoint_coef,
          const double _weight_diff_coef)
  {
    int excess_num = 0, disjoint_num = 0;
    GeneticEncoding::ExcessDisjoint(
            _genotype1,
            _genotype2,
            excess_num,
            disjoint_num);
    size_t num_genes = (std::max)(
            _genotype1->NumGenes(),
            _genotype2->NumGenes());
    std::vector< std::pair< GenePtr, GenePtr>> pairs = GeneticEncoding::Pair(
            _genotype1->SortedGenes(),
            _genotype2->SortedGenes());
    double weight_diff = 0;
    int count = 0;
    for (std::pair< GenePtr, GenePtr > pair : pairs)
    {
      if (pair.first != nullptr && pair.second != nullptr)
      {
        if (pair.first->type_ == Gene::CONNECTION_GENE
            && pair.second->type_ == Gene::CONNECTION_GENE)
        {
          weight_diff +=
                  std::abs(boost::dynamic_pointer_cast< ConnectionGene >(
                          pair.first)->weight_
                           - boost::dynamic_pointer_cast< ConnectionGene >(
                                   pair.second)->weight_);
          count++;
        }
      }
    }
    double average_weight_diff = count > 0 ? weight_diff / count : 0;
    double dissimilarity =
            (_disjoint_coef * disjoint_num + _excess_coef * excess_num)
            / num_genes + _weight_diff_coef * average_weight_diff;
    return dissimilarity;
  }

  void GeneticEncoding::ExcessDisjoint(
          GeneticEncodingPtr genotype1,
          GeneticEncodingPtr genotype2,
          int &excess_num,
          int &disjoint_num)
  {
    std::vector< GenePtr > genes_sorted1 = genotype1->SortedGenes();
    std::vector< GenePtr > genes_sorted2 = genotype2->SortedGenes();

    size_t min_mark1 = genes_sorted1[0]->InnovationNumber();
    size_t max_mark1 =
            genes_sorted1[genes_sorted1.size() - 1]->InnovationNumber();

    size_t min_mark2 = genes_sorted2[0]->InnovationNumber();
    size_t max_mark2 =
            genes_sorted2[genes_sorted2.size() - 1]->InnovationNumber();

    std::vector< std::pair< GenePtr, GenePtr>>
            pairs = GeneticEncoding::Pair(
            genes_sorted1,
            genes_sorted2);

    for (std::pair< GenePtr, GenePtr > pair : pairs)
    {
      if (pair.first != nullptr && pair.second == nullptr)
      {
        if (pair.first->InnovationNumber() > (min_mark2 - 1)
            && pair.first->InnovationNumber() < (max_mark2 + 1))
        {
          disjoint_num++;
        }
        else
        {
          excess_num++;
        }
      }
      else if (pair.first == nullptr && pair.second != nullptr)
      {
        if (pair.second->InnovationNumber() > (min_mark1 - 1)
            && pair.second->InnovationNumber() < (max_mark1 + 1))
        {
          disjoint_num++;
        }
        else
        {
          excess_num++;
        }
      }
    }
  }

  bool GeneticEncoding::ExistingNeuron(const size_t _innovationNumber)
  {
    if (not this->isLayered_)
    {
      for (const auto gene : this->neuronGenes_)
      {
        if (gene->InnovationNumber() == _innovationNumber)
        {
          return true;
        }
      }
      return false;
    }
    else
    {
      for (const auto layer : this->layers_)
      {
        for (const auto neuron_gene : layer)
        {
          if (neuron_gene->InnovationNumber() == _innovationNumber)
          {
            return true;
          }
        }
      }
      return false;
    }
  }

  bool GeneticEncoding::ExistingConnection(
          const size_t _from,
          const size_t _to)
  {
    for (auto connection : connectionGenes_)
    {
      if ((*connection).from_ == _from
          && (*connection).to_ == _to
          && (*connection).IsEnabled())
      {
        return true;
      }
    }
    return false;
  }

  GenePtr GeneticEncoding::Find(const size_t _innovationNumber)
  {
    this->SortedGenes();
    for (GenePtr gene : this->allSortedGenes_)
    {
      if (gene->InnovationNumber() == _innovationNumber)
      {
        return gene;
      }
    }
    return nullptr;
  }

  size_t GeneticEncoding::NumConnections()
  {
    return this->connectionGenes_.size();
  }

  size_t GeneticEncoding::NumGenes()
  {
    if (not isLayered_)
    {
      return this->NumNeurons() + this->NumConnections();
    }
    else
    {
      size_t sum = 0;
      for (const auto layer : layers_)
      {
        sum += layer.size();
      }
      return sum + this->NumConnections();
    }
  }

  size_t GeneticEncoding::NumNeurons()
  {
    if (not isLayered_)
    {
      return this->NumLayers();
    }
    else
    {
      size_t sum = 0;
      for (const auto layer : layers_)
      {
        sum += layer.size();
      }
      return sum;
    }
  }

  size_t GeneticEncoding::NumLayers()
  {
    return this->neuronGenes_.size();
  }

  std::vector< std::pair< GenePtr, GenePtr > > GeneticEncoding::Pair(
          std::vector< GenePtr > _sortedGenes1,
          std::vector< GenePtr > _sortedGenes2)
  {
    size_t num_genes1 = _sortedGenes1.size();
    size_t num_genes2 = _sortedGenes2.size();

    size_t min_mark1 = _sortedGenes1[0]->InnovationNumber();
    size_t max_mark1 =
            _sortedGenes1[_sortedGenes1.size() - 1]->InnovationNumber();

    size_t min_mark2 = _sortedGenes2[0]->InnovationNumber();
    size_t max_mark2 =
            _sortedGenes2[_sortedGenes2.size() - 1]->InnovationNumber();

    size_t min_mark = (std::min)(min_mark1, min_mark2);
    size_t max_mark = (std::max)(max_mark1, max_mark2);

    std::vector< std::pair< GenePtr, GenePtr>> gene_pairs;

    // search for pairs with equal marks
    size_t start_from1 = 0;
    size_t start_from2 = 0;

    size_t mark = min_mark;

    while (mark < max_mark + 1)
    {
      // jump1 and jump2 are here to skip long sequences of empty
      // historical marks
      GenePtr gene1 = nullptr;
      size_t jump1 = mark + 1;
      for (size_t i = start_from1; i < num_genes1; i++)
      {
        if (_sortedGenes1[i]->InnovationNumber() == mark)
        {
          gene1 = _sortedGenes1[i];
          start_from1 = i;
          break;
        }
        else if (_sortedGenes1[i]->InnovationNumber() > mark)
        { // if there is a gap jump over it
          jump1 = _sortedGenes1[i]->InnovationNumber();
          start_from1 = i;
          break;
        }
        else if (i == num_genes1 - 1)
        { // if the end of the gene sequence is reached
          jump1 = max_mark + 1;
          start_from1 = i;
          break;
        }
      }

      GenePtr gene2 = nullptr;
      size_t jump2 = mark + 1;
      for (size_t i = start_from2; i < num_genes2; i++)
      {
        if (_sortedGenes2[i]->InnovationNumber() == mark)
        {
          gene2 = _sortedGenes2[i];
          start_from2 = i;
          break;
        }
        else if (_sortedGenes2[i]->InnovationNumber() > mark)
        { // if there is a gap jump over it
          jump2 = _sortedGenes2[i]->InnovationNumber();
          start_from2 = i;
          break;
        }
        else if (i == num_genes2 - 1)
        { // if the end of the gene sequence is reached
          jump2 = max_mark + 1;
          start_from2 = i;
          break;
        }
      }

      if (gene1 != nullptr || gene2 != nullptr)
      {
        gene_pairs.push_back(std::pair< GenePtr, GenePtr >(gene1, gene2));
      }

      mark = std::min(jump1, jump2);
    }
    return gene_pairs;
  }

  std::pair< int, int > GeneticEncoding::RangeInnovationNumbers()
  {
    SortedGenes();
    return std::pair< int, int >(
            allSortedGenes_[0]->InnovationNumber(),
            allSortedGenes_[allSortedGenes_.size() - 1]->InnovationNumber());
  }

  void GeneticEncoding::RemoveNeuron(const size_t _index)
  {
    GenePtr oldGene = boost::dynamic_pointer_cast< Gene >
            (this->neuronGenes_.at(_index));
    this->neuronGenes_.erase(this->neuronGenes_.begin() + _index);
    auto gene = std::find(
            this->allSortedGenes_.begin(),
            this->allSortedGenes_.end(),
            oldGene);
    this->allSortedGenes_.erase(gene);
  }

  void GeneticEncoding::RemoveNeuron(NeuronGenePtr _gene)
  {
    std::cout << "BEGIN removing neuron" << std::endl;
    auto gene = std::find(
            this->neuronGenes_.begin(),
            this->neuronGenes_.end(),
            _gene);
    this->neuronGenes_.erase(gene);
    auto sortedGene = std::find(
            this->allSortedGenes_.begin(),
            this->allSortedGenes_.end(),
            _gene);
    std::cout << "END removing neuron" << std::endl;

    this->allSortedGenes_.erase(sortedGene);
    std::cout << "END removing neuron" << std::endl;

  }

  void GeneticEncoding::RemoveNeuron(
          const size_t _layer,
          const size_t _index)
  {
    GenePtr oldGene = boost::dynamic_pointer_cast< Gene >
            (this->layers_.at(_layer).at(_index));
    if (this->layers_.at(_layer).size() == 1)
    {
      this->layers_.erase(layers_.begin() + _layer);
    }
    else
    {
      this->layers_.at(_layer).erase(
              this->layers_.at(_layer).begin() + _index);
    }
    auto gene = std::find(
            this->allSortedGenes_.begin(),
            allSortedGenes_.end(),
            oldGene);
    this->allSortedGenes_.erase(gene);
  }

  void GeneticEncoding::RemoveConnection(const size_t _index)
  {
    GenePtr old = boost::dynamic_pointer_cast< Gene >(connectionGenes_[_index]);
    connectionGenes_.erase(connectionGenes_.begin() + _index);
    auto it = std::find(allSortedGenes_.begin(), allSortedGenes_.end(), old);
    allSortedGenes_.erase(it);
  }

  std::vector< GenePtr > GeneticEncoding::SortedGenes()
  {
    if (not all_genes_valid)
    {
      allSortedGenes_.clear();
      if (not isLayered_)
      {
        for (const auto neuron_gene : neuronGenes_)
        {
          allSortedGenes_.push_back(
                  boost::dynamic_pointer_cast< Gene >(neuron_gene));
        }
        for (const auto connection_gene : connectionGenes_)
        {
          allSortedGenes_.push_back(
                  boost::dynamic_pointer_cast< Gene >(connection_gene));
        }
      }
      else
      {
        for (std::vector< NeuronGenePtr > layer : layers_)
        {
          for (const auto neuron_gene : layer)
          {
            allSortedGenes_.push_back(
                    boost::dynamic_pointer_cast< Gene >(neuron_gene));
          }
        }
        for (const auto connection_gene : connectionGenes_)
        {
          allSortedGenes_.push_back(boost::dynamic_pointer_cast< Gene >(
                  connection_gene));
        }
      }
      std::sort(allSortedGenes_.begin(), allSortedGenes_.end(), CompareGenes);
      all_genes_valid = true;
    }
    return allSortedGenes_;
  }

  std::vector< std::pair< int, int > > GeneticEncoding::SpaceMap(
          std::vector< GeneticEncodingPtr > _genotypes,
          std::map< Neuron::Ntype, Neuron::NeuronTypeSpec > _config)
  {
    std::vector< std::vector< GenePtr>> sortedGenes;
    for (const auto genotype : _genotypes)
    {
      sortedGenes.push_back(genotype->SortedGenes());
    }

    size_t globMinIn = sortedGenes[0][0]->InnovationNumber();
    size_t globMaxIn =
            sortedGenes[0][sortedGenes.size() - 1]->InnovationNumber();

    for (std::vector< GenePtr > genes : sortedGenes)
    {
      size_t minIn = genes.at(0)->InnovationNumber();
      size_t maxIn = genes.at(genes.size() - 1)->InnovationNumber();

      if (minIn < globMinIn)
      {
        globMinIn = minIn;
      }
      if (maxIn > globMaxIn)
      {
        globMaxIn = maxIn;
      }
    }

    std::vector< std::pair< int, int>> innovationNumbers;
    // TODO: check of 0 is necessary
    // TODO: make this faster
    for (size_t inNum = 0; inNum <= globMaxIn; inNum++)
    {
      GenePtr currentGene = nullptr;
      for (std::vector< GenePtr > gene_vector : sortedGenes)
      {
        bool total_break = false;
        for (const auto gene : gene_vector)
        {
          if (gene->InnovationNumber() == inNum)
          {
            currentGene = gene;
            total_break = true;
            break;
          }
          else if (gene->InnovationNumber() > inNum)
          {
            break;
          }
        }
        if (total_break)
        {
          break;
        }
      }
      if (currentGene == nullptr)
      {
        innovationNumbers.push_back(std::pair< int, int >(inNum, 0));
      }
      else
      {
        switch (currentGene->type_)
        {
          case Gene::CONNECTION_GENE:
            innovationNumbers.push_back(std::pair< int, int >(inNum, 1));
            break;
          case Gene::NEURON_GENE:
            Neuron::Ntype neuron_type = boost::dynamic_pointer_cast< NeuronGene >(
                    currentGene)->neuron_->neuronType_;
            Neuron::NeuronTypeSpec neuron_spec = _config[neuron_type];
            innovationNumbers.push_back(std::pair< int, int >(
                    inNum,
                    neuron_spec.parameters.size()));
            break;
        }
      }
    }
    return innovationNumbers;
  }

#ifdef CPPNEAT_DEBUG
  bool GeneticEncoding::is_valid()
  {
      if (not layered)
      {
          for (ConnectionGenePtr connection_gene : connection_genes)
           {
              if (not neuron_exists(connection_gene->mark_from)
              || not neuron_exists(connection_gene->mark_to))
               {
                  if (not neuron_exists(connection_gene->mark_from))
                   {
                      std::cerr << "neuron with mark "
                      << connection_gene->mark_from
                       << " doesnt exist" <<std::endl;
                  }
                  if (not neuron_exists(connection_gene->mark_to))
                   {
                      std::cerr << "neuron with mark "
                       << connection_gene->mark_to
                       << " doesnt exist" <<std::endl;
                  }
                  return false;
              }
          }
      }
      else
      {
          for (ConnectionGenePtr connection_gene : connection_genes)
           {
              if (not neuron_exists(connection_gene->mark_from)
              || not neuron_exists(connection_gene->mark_to))
              {
                  if (not neuron_exists(connection_gene->mark_from))
                  {
                      std::cerr << "neuron with mark "
                      << connection_gene->mark_from
                      << " doesnt exist" <<std::endl;
                  }
                  if (not neuron_exists(connection_gene->mark_to))
                  {
                      std::cerr << "neuron with mark "
                      << connection_gene->mark_to
                      << " doesnt exist" <<std::endl;
                  }
                  return false;
              }
              if (convert_in_to_layer_index(connection_gene->mark_from).first
              >= convert_in_to_layer_index(connection_gene->mark_to).first)
              {
                  std::cerr << "layer of neuron with in "
                  << connection_gene->mark_from
                  << " is geq than layer of neuron with in "
                   << connection_gene->mark_to << std::endl;
                  return false;
              }
          }
      }
      return true;
  }
#endif
}
