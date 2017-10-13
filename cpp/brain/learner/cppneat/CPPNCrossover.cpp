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
#include <random>

#include "CPPNCrossover.h"

namespace cppneat
{
  GeneticEncodingPtr Crossover::crossover(
          GeneticEncodingPtr _genotype1,
          GeneticEncodingPtr _genotype2)
  {
    assert(_genotype2->isLayered_ == _genotype1->isLayered_);
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution< double > udist(0, 1);
    _genotype1 = _genotype1->Copy();
    _genotype2 = _genotype2->Copy();

    auto betterGenes = _genotype1->SortedGenes();
    auto worseGenes = _genotype2->SortedGenes();
    auto genePairs = GeneticEncoding::Pair(betterGenes, worseGenes);

    std::vector< GenePtr > offspring;
    for (auto pair : genePairs)
    {
      if (pair.first not_eq nullptr
          and pair.second not_eq nullptr)
      {
        (udist(mt) < 0.5)
        ? offspring.push_back(pair.first)
        : offspring.push_back(pair.second);
      }
      else if (pair.first not_eq nullptr)
      {
        offspring.push_back(pair.first);
      }
    }

    GeneticEncodingPtr childGenotype;
    if (not _genotype2->isLayered_)
    {
      childGenotype = GeneticEncodingPtr(new GeneticEncoding(false));
      for (auto gene : offspring)
      {
        switch (gene->type_)
        {
          case Gene::NEURON_GENE:
            childGenotype->AddNeuron(
                    boost::dynamic_pointer_cast< NeuronGene >(gene));
            break;
          case Gene::CONNECTION_GENE:
            childGenotype->AddConnection(
                    boost::dynamic_pointer_cast< ConnectionGene >(gene));
            break;
        }
      }
    }
    else
    {
      // what helps us tremendously here is the fact that a gene is only in the
      // child if it is in the more fit parent therefore we can use the same
      // layer structure as in the more fit parent here
      _genotype1->connectionGenes_.clear();
      for (auto gene : offspring)
      {
        switch (gene->type_)
        {
          case Gene::NEURON_GENE:
          {
            if (not _genotype1->ExistingNeuron(gene->InnovationNumber()))
            {
              std::cout << "BEGIN Neuron NOT exists" << std::endl;
              _genotype1->AddNeuron(
                      boost::dynamic_pointer_cast< NeuronGene >(gene),
                      1, false);
              std::cout << "END Neuron NOT exists" << std::endl;
            }
            auto index = _genotype1->CoordinatesByInnovationNumber(
                    gene->InnovationNumber());
            _genotype1->layers_.at(index.first).at(index.second) =
                    boost::dynamic_pointer_cast< NeuronGene >(gene);
            break;
          }
          case Gene::CONNECTION_GENE:
          {
            _genotype1->AddConnection(
                    boost::dynamic_pointer_cast< ConnectionGene >(gene));
            break;
          }
        }
      }
      childGenotype = _genotype1;
    }

    return childGenotype;
  }
}
