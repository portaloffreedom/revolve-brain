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
  typedef std::vector< GeneticEncodingPtr > GeneticEncodingPtrs;

  typedef std::pair< GeneticEncodingPtr, GeneticEncodingPtr > ParentPair;

  typedef std::vector< ParentPair > ParentPairs;

  typedef std::pair< GeneticEncodingPtr, GeneticEncodingPtrs >
          ParentVectorPairs;

  typedef std::pair< GeneticEncodingPtr, double > FitnessPair;

  typedef std::vector< FitnessPair > FitnessPairs;

  class NEATLearner
          : public revolve::brain::Learner< GeneticEncodingPtr >
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
    NEATLearner(
            MutatorPtr _mutator,
            const std::string &_mutatorPath,
            LearningConfiguration &_configuration);

    /// \brief
    void Initialise(GeneticEncodingPtrs _genotypes);

    /// \brief
    GeneticEncodingPtrs InitBrains();

    /// \brief
    GeneticEncodingPtrs YamlBrains(
            const std::string &_yamlPath,
            const int _offset);

    /// \brief
    void ApplyStructuralMutation(GeneticEncodingPtr _genotype);

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
    virtual void reportFitness(
            const std::string &_id,
            GeneticEncodingPtr _genotype,
            const double _fitness);

    /// \brief
    virtual GeneticEncodingPtr currentGenotype();

    /// \brief
    void RecordGenome(
            const std::string &_robotName,
            GeneticEncodingPtr _genome);

    /// \brief
    void ShareFitness();

    /// \brief
    void Population();

    /// \brief
    void Reproduce(std::map< GeneticEncodingPtr, size_t > _offsprings);

    /// \brief
    std::map< GeneticEncodingPtr, size_t > NumChildrenPerSpecie();

    /// \brief
    GeneticEncodingPtr ProduceChild(
            GeneticEncodingPtr _parent1,
            GeneticEncodingPtr _parent2);

    /// \brief
    ParentPair TournamentSelection(
            FitnessPairs _candidates,
            size_t _tournamentSize);

    /// \brief
    GeneticEncodingPtr activeBrain_;

    //    double fitness;
    /// \brief
    std::vector< GeneticEncodingPtr > evaluationQueue_;

    /// \brief
    GeneticEncodingPtrs brainPpopulation_;

    /// \brief
    std::map< GeneticEncodingPtr, double > brainFitness_;

    /// \brief
    std::map< GeneticEncodingPtr, double > brainVelocity_;

    /// \brief
    std::map< GeneticEncodingPtr, std::vector< GeneticEncodingPtr>> species_;

    /// \brief
    std::vector< double > fitnessBuffer_;

    /// \brief
    int numGeneration;

    /// \brief
    int numEvaluatedBrains;

    /// \brief
    MutatorPtr mutator_;

    /// \brief
    std::string mutatorPath_;

    /// \brief
    bool isAsexual_;

    /// \brief
    size_t initStructuralMutations_;

    /// \brief
    size_t numChildren_;

    /// \brief
    size_t populationSize_;

    /// \brief
    size_t tournamentSize_;

    /// \brief
    double weightMutationProbability_;

    /// \brief
    double weightMutationSigma_;

    /// \brief
    double paramMutationProbability_;

    /// \brief
    double paramMutationSigma_;

    /// \brief
    double augmentationProbability_;

    /// \brief
    double removalProbability_;

    /// \brief
    int maxGenerations_;

    /// \brief
    double speciationThreshold_;

    /// \brief
    size_t repeatEvaluation_;

    /// \brief
    GeneticEncodingPtr startFrom_;

    /// \brief
    double interspeciesMateProbability_;

    /// \brief
    std::mt19937 generator;
  };
}

#endif  // REVOLVEBRAIN_BRAIN_LEARNER_NEATLEARNER_H_
