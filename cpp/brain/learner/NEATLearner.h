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

#ifndef REVOLVEBRAIN_BRAIN_LEARNER_NEATLEARNER_H_
#define REVOLVEBRAIN_BRAIN_LEARNER_NEATLEARNER_H_

// #define CPPNEAT_DEBUG

#include <map>
#include <string>
#include <vector>
#include <random>
#include <utility>

#include "Learner.h"
#include "brain/learner/cppneat/CPPNTypes.h"
#include "brain/learner/cppneat/CPPNMutator.h"

/// \brief crossover between genotypes
namespace cppneat
{
  class NEATLearner
          : public revolve::brain::Learner<GeneticEncodingPtr>
  {
    public:
    struct LearningConfiguration
    {
      bool asexual;
      int pop_size;
      int tournament_size;
      int num_children;
      double weight_mutation_probability;
      double weight_mutation_sigma;
      double param_mutation_probability;
      double param_mutation_sigma;
      double structural_augmentation_probability;
      double structural_removal_probability;
      int max_generations;
      double speciation_threshold;
      int repeat_evaluations;
      GeneticEncodingPtr start_from;
      int initial_structural_mutations;
      double interspecies_mate_probability;
    };

    /// \brief
    NEATLearner(MutatorPtr mutator,
                std::string mutator_path,
                LearningConfiguration conf);

    /// \brief
    void initialise(std::vector<GeneticEncodingPtr> init_genotypes);

    /// \brief
    std::vector<GeneticEncodingPtr> get_init_brains();

    /// \brief
    std::vector<GeneticEncodingPtr> get_brains_from_yaml(std::string yaml_path,
                                                         int offset);

    /// \brief
    void apply_structural_mutation(GeneticEncodingPtr genotype);

    // standard parameters
    static const bool ASEXUAL;
    static const int POP_SIZE;
    static const int TOURNAMENT_SIZE;
    static const int NUM_CHILDREN;
    static const double WEIGHT_MUTATION_PROBABILITY;
    static const double WEIGHT_MUTATION_SIGMA;
    static const double PARAM_MUTATION_PROBABILITY;
    static const double PARAM_MUTATION_SIGMA;
    static const double STRUCTURAL_AUGMENTATION_PROBABILITY;
    static const double STRUCTURAL_REMOVAL_PROBABILITY;
    static const int MAX_GENERATIONS;
    static const double SPECIATION_TRESHOLD;
    static const int REPEAT_EVALUATIONS;
    static const int INITIAL_STRUCTURAL_MUTATIONS;
    static const double INTERSPECIES_MATE_PROBABILITY;
    private:
    /// \brief
    virtual void reportFitness(std::string id,
                               GeneticEncodingPtr genotype,
                               double fitness);

    /// \brief
    virtual GeneticEncodingPtr currentGenotype();

    /// \brief
    void writeGenome(std::string &robot_name,
                     GeneticEncodingPtr genome);

    /// \brief
    void share_fitness();

    /// \brief
    void produce_new_generation();

    /// \brief
    GeneticEncodingPtr produce_child(GeneticEncodingPtr parent1,
                                     GeneticEncodingPtr parent2);

    /// \brief
    std::pair<GeneticEncodingPtr, GeneticEncodingPtr>
    select_for_tournament(
            std::vector<std::pair<GeneticEncodingPtr, double>> candidates,
            unsigned int tourn_size);

    /// \brief
    GeneticEncodingPtr active_brain_;

    //    double fitness;
    /// \brief
    std::vector<GeneticEncodingPtr> evaluation_queue_;

    /// \brief
    std::vector<GeneticEncodingPtr> brain_population_;

    /// \brief
    std::map<GeneticEncodingPtr, double> brain_fitness;

    /// \brief
    std::map<GeneticEncodingPtr, double> brain_velocity;

    /// \brief
    std::map<GeneticEncodingPtr, std::vector<GeneticEncodingPtr>> species;

    /// \brief
    std::vector<double> fitness_buffer;

    /// \brief
    int generation_number;

    /// \brief
    int total_brains_evaluated;

    /// \brief
    MutatorPtr mutator;

    /// \brief
    std::string mutator_path;

    /// \brief
    bool is_asexual_;

    /// \brief
    size_t initial_structural_mutations_;

    /// \brief
    size_t num_children_;

    /// \brief
    size_t population_size_;

    /// \brief
    size_t tournament_size_;

    /// \brief
    double weight_mutation_probability;

    /// \brief
    double weight_mutation_sigma;

    /// \brief
    double param_mutation_probability;

    /// \brief
    double param_mutation_sigma;

    /// \brief
    double structural_augmentation_probability;

    /// \brief
    double structural_removal_probability;

    /// \brief
    int max_generations;

    /// \brief
    double speciation_threshold;

    /// \brief
    unsigned int repeat_evaluations;

    /// \brief
    GeneticEncodingPtr start_from_;

    /// \brief
    double interspecies_mate_probability;

    /// \brief
    std::mt19937 generator;
  };
}

#endif  // REVOLVEBRAIN_BRAIN_LEARNER_NEATLEARNER_H_
