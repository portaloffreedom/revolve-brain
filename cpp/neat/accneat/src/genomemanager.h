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

#pragma once

#include "neat.h"
#include "util/rng.h"
#include <memory>
#include <map>
#include <vector>

namespace NEAT {

class Genome;

class GenomeManager
{
public:
    static GenomeManager *
    create(const std::string &robot_name);

    virtual ~GenomeManager()
    {}

    virtual std::unique_ptr<Genome>
    make_default() = 0;

    virtual std::vector<std::unique_ptr<Genome>>
    create_seed_generation(size_t ngenomes,
                           class rng_t rng,
                           size_t ntraits,
                           size_t ninputs,
                           size_t noutputs,
                           size_t nhidden,
                           const std::string &robot_name) = 0;

    virtual bool
    are_compatible(Genome &genome1,
                   Genome &genome2) = 0;

    virtual void
    clone(Genome &orig,
          Genome &clone) = 0;

    virtual void
    mate(Genome &genome1,
         Genome &genome2,
         Genome &offspring,
         real_t fitness1,
         real_t fitness2) = 0;

    enum MutationOperation
    {
        MUTATE_OP_WEIGHTS,
        MUTATE_OP_STRUCTURE,
        MUTATE_OP_ANY
    };

    virtual void
    mutate(Genome &genome,
           MutationOperation op = MUTATE_OP_ANY) = 0;

    virtual void
    finalize_generation(bool new_fittest) = 0;
};

}
