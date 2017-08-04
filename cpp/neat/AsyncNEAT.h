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

#ifndef REVOLVE_NEAT_NEAT_H_
#define REVOLVE_NEAT_NEAT_H_

#include <list>
#include <memory>
#include <string>

#include "organism.h"
#include "population.h"

#include "NEATEvaluation.h"

/**
 * This is an asynchronous NEAT implemetation, it could be hard to understand
 *
 * The main loop cycle must be implemented outside (for more flexibility in real
 * time).
 *
 * To use this NEAT implemetation, get evaluations to complete using getEvaluation().
 * Then complete each evaluation calling Evaluation.finish(). If not all evaluations
 * are finished, the getEvaluation could fail.
 *
 * Multiple evaluation can be tested at the same time, is not a problem.
 *
 * It conflicts with the classical implemeation of accuNEAT since there are some
 * global objects to be used, be carefull.
 */
class AsyncNeat
{
  public:
  /// \brief
  AsyncNeat(unsigned int n_inputs,
            unsigned int n_outputs,
            int rng_seed,
            const std::string &robot_name);

  /// \brief
  virtual ~AsyncNeat();

  /// \brief If it returns nullptr, wait until all evaluations are finished to get
  /// the next generation
  std::shared_ptr<NeatEvaluation> getEvaluation();

  /// \brief to be called before any AsyncNeat object can be used
  /// \param robot_name robot_name to
  static void Init(const std::string &robot_name)
  {
    NEAT::env->genome_manager = NEAT::GenomeManager::create(robot_name);
  };


  /// \brief to be called before any AsyncNeat object can be used
  /// It initializes the environment with a personalized genome manager instead
  /// of the standard one. The delete of this object will be handled
  /// by AsyncNeat::CleanUp()
  static void Init(std::unique_ptr<NEAT::GenomeManager> genome_manager)
  {
    NEAT::env->genome_manager = genome_manager.release();
  };

  /// \brief to be called after all AsyncNeat object are not in use anymore
  static void CleanUp()
  {
    delete NEAT::env->genome_manager;
    NEAT::env->genome_manager = nullptr;
  };

  /// \brief
  static void SetSearchType(NEAT::GeneticSearchType type)
  {
    NEAT::env->search_type = type;
  }

  /// \brief
  static void SetPopulationSize(unsigned int n)
  {
    NEAT::env->pop_size = n;
  }

  /// \brief
  static void SetPopulationType(NEAT::PopulationType type)
  {
    NEAT::env->population_type = type;
  }

  /// \brief default is 0.01
  static void SetMutateAddNodeProb(NEAT::real_t value)
  {
    NEAT::env->mutate_add_node_prob = value;
  }

  /// \brief default is 0.3
  static void SetMutateAddLinkProb(NEAT::real_t value)
  {
    NEAT::env->mutate_add_link_prob = value;
  }

  /**
   * Set the prob. that a link mutation which doesn't have to be recurrent will be made recurrent
   /// \param prob new probability
   */
  static void SetRecurProb(NEAT::real_t prob)
  {
    NEAT::env->recur_prob = prob;
  }

  /// \brief Set the probability of forcing selection of ONLY links that are naturally recurrent
  /// \param prob new probability
  static void SetRecurOnlyProb(NEAT::real_t prob)
  {
    NEAT::env->recur_only_prob = prob;
  }

  /// \brief
  std::shared_ptr<NeatEvaluation> getFittest() const
  {
    return fittest;
  }

  protected:
  /// \brief
  void setFittest(std::shared_ptr<NeatEvaluation> new_fittest,
                  float new_fitness);

  private:
  /// \brief
  unsigned int n_inputs;

  /// \brief
  unsigned int n_outputs;

  /// \brief
  unsigned int generation;

  /// \brief
  unsigned int best_fitness_counter;

  /// \brief
  int rng_seed;

  /// \brief
  NEAT::Population *population;

  /// \brief
  std::list<std::shared_ptr<NeatEvaluation>> evaluatingList;

  /// \brief
  std::list<std::shared_ptr<NeatEvaluation>> evaluatingQueue;

  /// \brief
  std::shared_ptr<NeatEvaluation> fittest;

  /// \brief
  float fittest_fitness;

  /// \brief
  const std::string robot_name;

  /// \brief
  void singleEvalutionFinished(std::shared_ptr<NeatEvaluation> evaluation,
                               float fitness);

  /// \brief
  void next_generation();

  /// \brief
  void refill_evaluation_queue();
};

#endif  //  REVOLVE_NEAT_NEAT_H_
