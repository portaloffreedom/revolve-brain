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

#ifndef REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_GENETICENCODING_H_
#define REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_GENETICENCODING_H_

#include <map>
#include <utility>
#include <vector>

#include "CPPNTypes.h"
#include "CPPNNeuron.h"
#include "ConnectionGenome.h"
#include "NeuronGenome.h"

/// \brief class for the encoding of one genotype
namespace cppneat
{
  class GeneticEncoding
  {
    public:
    /// \brief
    GeneticEncoding(std::vector<NeuronGenePtr> neuron_genes,
                    std::vector<ConnectionGenePtr> connection_genes)
            : neuronGenes_(neuron_genes)
            , connectionGenes_(connection_genes)
            , isLayered_(false)
    {}

    /// \brief
    GeneticEncoding(std::vector<std::vector<NeuronGenePtr>> layers,
                    std::vector<ConnectionGenePtr> connection_genes)
            : layers_(layers)
            , connectionGenes_(connection_genes)
            , isLayered_(true)
    {}

    /// \brief
    GeneticEncoding(bool layered)
            : isLayered_(layered)
    {}

    /// \brief
    GeneticEncodingPtr Copy();

    /// \brief
    size_t NumGenes();

    /// \brief
    size_t NumNeurons();

    /// \brief
    size_t NumConnections();

    /// \brief
    size_t NumLayers();

    /// \brief
    bool ExistingConnection(
            const size_t _from,
            const size_t _to);

    /// \brief
    static double Dissimilarity(
            GeneticEncodingPtr _genotype1,
            GeneticEncodingPtr _genotype2,
            const double _excess_coef,
            const double _disjoint_coef,
            const double _weight_diff_coef);

    /// \brief
    static void ExcessDisjoint(
            GeneticEncodingPtr genotype1,
            GeneticEncodingPtr genotype2,
            int &excess_num,
            int &disjoint_num);

    /// \brief
    static std::vector<std::pair<GenePtr, GenePtr>>
    Pair(
            std::vector< GenePtr > _sortedGenes1,
            std::vector< GenePtr > _sortedGenes2);

    /// \brief
    static std::vector<std::pair<int, int>>
    SpaceMap(
            std::vector< GeneticEncodingPtr > _genotypes,
            std::map< Neuron::Ntype, Neuron::NeuronTypeSpec > _config);

    /// \brief
    void Adopt(GeneticEncodingPtr adoptee);

    /// \brief
    std::vector<GenePtr> SortedGenes();

    /// \brief
    std::pair<int, int> RangeInnovationNumbers();

    /// \brief
    GenePtr Find(const size_t _innovationNumber);

    /// \brief non-layered
    void AddNeuron(NeuronGenePtr _neuron);

    /// \brief layered
    void AddNeuron(
            NeuronGenePtr _neuron,
            const size_t _layer,
            const bool _newLayer);

    /// \brief
    void AddConnection(ConnectionGenePtr _connection);

    /// \brief
    void RemoveConnection(const size_t _index);

    /// \brief non-layered
    void RemoveNeuron(const size_t _index);

    /// \brief
    void RemoveNeuron(NeuronGenePtr _gene);

    /// \brief layered
    void RemoveNeuron(
            const size_t _layer,
            const size_t _index);

    /// \brief
    bool ExistingNeuron(const size_t _innovationNumber);

#ifdef CPPNEAT_DEBUG
    bool is_valid();
#endif

    /// \brief
    std::pair<size_t, size_t> CoordinatesByIndex(const size_t _index);

    /// \brief
    std::pair<size_t, size_t>
    CoordinatesByInnovationNumber(const size_t _innovationNumber);

    public:
    /// \brief non-layered
    public:
    /// \brief
    std::vector<NeuronGenePtr> neuronGenes_;

    public:
    /// \brief layered
    std::vector<std::vector<NeuronGenePtr>> layers_;

    public:
    /// \brief both
    std::vector<ConnectionGenePtr> connectionGenes_;

    public:
    /// \brief
    bool isLayered_;

    private:
    /// \brief
    std::vector<GenePtr> allSortedGenes_;

    private:
    /// \brief
    bool all_genes_valid;
  };
}

#endif  //  REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_GENETICENCODING_H_
