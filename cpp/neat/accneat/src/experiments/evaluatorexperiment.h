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

#ifndef CPP_NEAT_ACCNEAT_SRC_EXPERIMENTS_EVALUATOREXPERIMENT_H_
#define CPP_NEAT_ACCNEAT_SRC_EXPERIMENTS_EVALUATOREXPERIMENT_H_

#pragma once

#include <fstream>
#include <functional>
#include <string>
#include <vector>

#include "genomemanager.h"
#include "experiment.h"
#include "organism.h"
#include "population.h"
#include "network/network.h"
#include "util/stats.h"
#include "util/timer.h"
#include "util/util.h"

namespace NEAT
{
  ////------------------------
  ///
  /// CLASS EvaluatorExperiment
  ///
  ////------------------------
  class EvaluatorExperiment
          : public Experiment
  {
    private:
    std::string get_dir_path(int experiment_num)
    {
      char buf[1024];
      std::snprintf(buf, "./experiment_%d", experiment_num);
      return buf;
    }

    std::string get_fittest_path(
            int experiment_num,
            int generation)
    {
      char buf[1024];
      std::snprintf(buf, "%s/fittest_%d",
                    get_dir_path(experiment_num).c_str(), generation);
      return buf;
    }

    public:
    typedef std::function< NetworkEvaluator *() > CreateEvaluatorFunc;
    typedef std::function< std::vector< std::unique_ptr< Genome >>(rng_t rng) >
            CreateSeedsFunc;

    CreateEvaluatorFunc create_evaluator;
    CreateSeedsFunc create_seeds;
    std::unique_ptr< NetworkEvaluator > network_evaluator;

    EvaluatorExperiment(
            const char *name,
            CreateEvaluatorFunc create_evaluator_,
            CreateSeedsFunc create_seeds_)
            : Experiment(name)
            , create_evaluator(create_evaluator_)
            , create_seeds(create_seeds_)
    {
    }

    virtual ~EvaluatorExperiment()
    {
    }

    virtual bool is_success(Organism *org)
    {
      return org->eval.error <= 0.0000001;
    }

    virtual void run(
            class rng_t &rng,
            int gens) override
    {
      using namespace std;

      network_evaluator = unique_ptr< NetworkEvaluator >(create_evaluator());

      int nsuccesses = 0;
      vector< int > success_generations;
      vector< size_t > nnodes;
      vector< size_t > nlinks;
      vector< real_t > fitness;

      for (size_t expcount = 1; expcount <= env->num_runs; expcount++)
      {
        mkdir(get_dir_path(expcount));

        // Create a unique rng sequence for this experiment
        rng_t rng_exp(rng.integer());

        fittest = nullptr;
        env->genome_manager = GenomeManager::create(get_name());
        vector< unique_ptr< Genome>> genomes = create_seeds(rng_exp);

        // Spawn the Population
        pop = Population::create(rng_exp, genomes);

        bool success = false;
        int gen;
        for (gen = 1; !success and (gen <= gens); gen++)
        {
          cout
                  << "Epoch "
                  << gen
                  << " . Experiment "
                  << expcount
                  << "/"
                  << env->num_runs
                  << endl;

          static Timer timer("epoch");
          timer.start();

          if (gen not_eq 1)
          {
            pop->next_generation();
          }

          evaluate();

          if (is_success(fittest.get()))
          {
            success = true;
            nsuccesses++;
          }

          timer.stop();
          Timer::report();
        }

        if (success)
        {
          success_generations.push_back(gen);
        }
        {
          Genome::Stats gstats = fittest->genome->get_stats();
          fitness.push_back(fittest->eval.fitness);
          nnodes.push_back(gstats.nnodes);
          nlinks.push_back(gstats.nlinks);
        }

        delete pop;
        delete env->genome_manager;
      }

      cout
              << "Failures: "
              << (env->num_runs - nsuccesses)
              << " out of "
              << env->num_runs
              << " runs"
              << endl;
      if (not success_generations.empty())
      {
        cout << "Success generations: " << stats(success_generations) << endl;
      }
      cout << "fitness stats: " << stats(fitness) << endl;
      cout << "nnodes stats: " << stats(nnodes) << endl;
      cout << "nlinks stats: " << stats(nlinks) << endl;
    }

    private:
    //    void
    //    print(int experiment_num,
    //          int generation)
    //    {
    //      using namespace std;
    //
    //      ofstream out(get_fittest_path(experiment_num,
    //                                    generation));
    //      fittest->write(out);
    //    }

    void evaluate()
    {
      using namespace std;

      static Timer timer("evaluate");
      timer.start();

      size_t norgs = pop->size();
      Network *nets[norgs];
      for (size_t i = 0; i < norgs; i++)
      {
        nets[i] = pop->get(i)->net.get();
      }
      auto *evaluations = new OrganismEvaluation[norgs];

      network_evaluator->execute(nets, evaluations, norgs);

      Organism *best = nullptr;
      for (size_t i = 0; i < norgs; i++)
      {
        Organism *org = pop->get(i);
        org->eval = evaluations[i];
        if (not best or (org->eval.fitness > best->eval.fitness))
        {
          best = org;
        }
      }
      delete[]evaluations;

      timer.stop();

      if (not fittest or (best->eval.fitness > fittest->eval.fitness))
      {
        fittest = pop->make_copy(best->population_index);
      }

      Genome::Stats gstats = fittest->genome->get_stats();
      cout << "fittest [" << fittest->population_index << "]"
           << ": fitness=" << fittest->eval.fitness
           << ", error=" << fittest->eval.error
           << ", nnodes=" << gstats.nnodes
           << ", nlinks=" << gstats.nlinks
           << endl;
    }

    class Population *pop;

    std::unique_ptr< Organism > fittest;
  };
}

#endif
