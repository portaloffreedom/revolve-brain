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
            : neuron_genes_(neuron_genes)
              , connection_genes_(connection_genes)
              , is_layered_(false)
    {}

    /// \brief
    GeneticEncoding(std::vector<std::vector<NeuronGenePtr>> layers,
                    std::vector<ConnectionGenePtr> connection_genes)
            : layers_(layers)
              , connection_genes_(connection_genes)
              , is_layered_(true)
    {}

    /// \brief
    GeneticEncoding(bool layered)
            : is_layered_(layered)
    {}

    /// \brief
    GeneticEncodingPtr copy();

    /// \brief
    size_t num_genes();

    /// \brief
    size_t num_neuron_genes();

    /// \brief
    size_t num_connection_genes();

    /// \brief
    bool connection_exists(int mark_from,
                           int mark_to);

    /// \brief
    static double get_dissimilarity(GeneticEncodingPtr genotype1,
                                    GeneticEncodingPtr genotype2,
                                    double excess_coef,
                                    double disjoint_coef,
                                    double weight_diff_coef);

    /// \brief
    static void get_excess_disjoint(GeneticEncodingPtr genotype1,
                                    GeneticEncodingPtr genotype2,
                                    int &excess_num,
                                    int &disjoint_num);

    /// \brief
    static std::vector<std::pair<GenePtr, GenePtr>>
    get_pairs(std::vector<GenePtr> genes_sorted1,
              std::vector<GenePtr> genes_sorted2);

    /// \brief
    static std::vector<std::pair<int, int>>
    get_space_map(std::vector<GeneticEncodingPtr> genotypes,
                  std::map<Neuron::Ntype, Neuron::NeuronTypeSpec> brain_spec);

    /// \brief
    void adopt(GeneticEncodingPtr adoptee);

    /// \brief
    std::vector<GenePtr> get_sorted_genes();

    /// \brief
    std::pair<int, int> min_max_innov_numer();

    /// \brief
    GenePtr find_gene_by_in(const size_t innov_number);

    /// \brief non-layered
    void add_neuron_gene(NeuronGenePtr neuron_gene);

    /// \brief layered
    void add_neuron_gene(NeuronGenePtr neuron_gene,
                         int layer,
                         bool is_new_layer);

    /// \brief
    void add_connection_gene(ConnectionGenePtr connection_gene);

    /// \brief
    void remove_connection_gene(int index);

    /// \brief non-layered
    void remonve_neuron_gene(int index);

    /// \brief layered
    void remove_neuron_gene(int layer,
                            int index);

    /// \brief
    bool neuron_exists(const size_t innov_number);

#ifdef CPPNEAT_DEBUG
    bool is_valid();
#endif

    /// \brief
    std::pair<size_t, size_t> convert_index_to_layer_index(size_t index);

    /// \brief
    std::pair<size_t, size_t>
    convert_in_to_layer_index(const size_t innov_number);

    public:
    /// \brief non-layered
    public:
    /// \brief
    std::vector<NeuronGenePtr> neuron_genes_;

    public:
    /// \brief layered
    std::vector<std::vector<NeuronGenePtr>> layers_;

    public:
    /// \brief both
    std::vector<ConnectionGenePtr> connection_genes_;

    public:
    /// \brief
    bool is_layered_;

    private:
    /// \brief
    std::vector<GenePtr> all_genes_sorted;

    private:
    /// \brief
    bool all_genes_valid;
  };
}

#endif  //  REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_GENETICENCODING_H_
