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

#ifndef REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_MUTATOR_H_
#define REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_MUTATOR_H_

#include <map>
#include <string>
#include <vector>
#include <random>
#include <utility>

#include "GeneticEncoding.h"

/// \brief class responsible for mutation
namespace cppneat
{
  class Mutator
  {
    public:
    /// \brief
    Mutator(std::map<Neuron::Ntype, Neuron::NeuronTypeSpec> brain_spec,
            double new_connection_sigma,
            int innovation_number,
            int max_attempts,
            std::vector<Neuron::Ntype> addable_neurons,
            bool layered);

    /// \brief
    static std::vector<Neuron::Ntype>
    get_addable_types(std::map<Neuron::Ntype,
                               Neuron::NeuronTypeSpec> brain_spec);

    /// \brief
    void make_starting_genotype_known(GeneticEncodingPtr genotype);

    /// \brief
    void load_known_innovations(std::string yaml_path);

    /// \brief
    void insert_conn_innovation(int from, int to, int inovation_number);

    /// \brief
    void insert_neuron_innovation(Neuron::Ntype ntype,
                                  int in_no);

    /// \brief
    void write_known_innovations(std::string yaml_path);

    /// \brief
    void mutate_neuron_params(GeneticEncodingPtr genotype,
                              double probability,
                              double sigma);

    /// \brief
    void mutate_weights(GeneticEncodingPtr genotype,
                        double probability,
                        double sigma);

    /// \brief
    void mutate_structure(GeneticEncodingPtr genotype,
                          double probability);

    /// \brief
    bool add_connection_mutation(GeneticEncodingPtr genotype,
                                 double sigma);

    /// \brief
    void add_neuron_mutation(GeneticEncodingPtr genotype,
                             double sigma);

    /// \brief
    void remove_connection_mutation(GeneticEncodingPtr genotype);

    /// \brief
    void remove_neuron_mutation(GeneticEncodingPtr genotype);

    /// \brief
    int add_neuron(NeuronPtr neuron,
                   GeneticEncodingPtr genotype,
                   ConnectionGenePtr split);

    /// \brief
    int add_connection(int mark_from,
                       int mark_to,
                       double weight,
                       GeneticEncodingPtr genotype,
                       std::string socket);

    /// \brief
    std::map<Neuron::Ntype, Neuron::NeuronTypeSpec> get_brain_spec()
    {
      return brain_spec;
    };

    /// \brief
    void set_current_innovation_number(int innov_numb)
    {
      this->innovation_number = innov_numb;
    };

    /// \brief <mark_from, mark_to> -> innovation_number
    private:
    std::map<std::pair<int, int>, int> connection_innovations;

    /// \brief contains all connections that ever existed!
    /// <innovation_number of split connection,
    /// neuron_type of added neuron>
    ///  ->
    /// innovation_numbers of added neurons
    /// since we want the ability to have multiple neurons of the same type
    /// added between two neurons
    /// it is necessary that we check if a new neuron has already been added to
    /// the current genotype
    /// if so we add another neuron
    /// in order to store all the innovation numbers we need a vector*/
    private:
    std::map<std::pair<int, Neuron::Ntype>, std::vector<int>>
            neuron_innovations;

    /// \brief contains only neurons that have been added by structural mutation
    private:
    std::map<Neuron::Ntype, Neuron::NeuronTypeSpec> brain_spec;

    /// \brief
    private:
    double new_connection_sigma;

    /// \brief
    private:
    int innovation_number;

    /// \brief
    private:
    int max_attempts;

    /// \brief
    private:
    std::vector<Neuron::Ntype> addable_neurons;

    /// \brief
    private:
    std::mt19937 generator;
  };
}

#endif  // NEAT_MUTATOR_H_
