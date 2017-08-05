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

#ifndef CPP_NEAT_ACCNEAT_SRC_INNOVGENOME_INNOVGENOMEMANAGER_H_
#define CPP_NEAT_ACCNEAT_SRC_INNOVGENOME_INNOVGENOMEMANAGER_H_

#pragma once

#include <string>
#include <vector>

#include "genomemanager.h"
#include "innovation.h"

namespace NEAT
{
  class InnovGenomeManager
          : public GenomeManager
  {
    friend class GenomeManager;

    protected:
    InnovGenomeManager(const std::string &robot_name);

    public:
    virtual ~InnovGenomeManager();

    virtual std::unique_ptr<Genome>
    make_default() override;

    using NEAT::GenomeManager::create_seed_generation;
    virtual std::vector<std::unique_ptr<Genome>>
    create_seed_generation(size_t ngenomes,
                           class rng_t rng,
                           size_t ntraits,
                           size_t ninputs,
                           size_t noutputs,
                           size_t nhidden,
                           const std::string &robot_name) override;

    virtual bool are_compatible(Genome &genome1,
                                Genome &genome2) override;

    virtual void clone(Genome &orig,
                       Genome &clone) override;

    virtual void mate(Genome &genome1,
                      Genome &genome2,
                      Genome &offspring,
                      real_t fitness1,
                      real_t fitness2) override;

    virtual void mutate(Genome &genome,
                        MutationOperation op = MUTATE_OP_ANY) override;

    virtual void finalize_generation(bool new_fittest) override;

    protected:
    CreateInnovationFunc

    create_innov_func(Genome &g);

    bool is_mate_allowed();

    bool is_add_allowed();

    bool is_delete_allowed();

    PopulationInnovations innovations;

    int generation;

    enum SearchPhase
    {
      UNDEFINED,
      COMPLEXIFY,
      PRUNE
    } search_phase;

    int search_phase_start;

    int max_phase_duration;

    const std::string robot_name;
  };
}

#endif
