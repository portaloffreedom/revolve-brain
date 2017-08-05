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

/*
 Copyright 2001 The University of Texas at Austin

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef CPP_NEAT_ACCNEAT_SRC_UTIL_MAP_H_
#define CPP_NEAT_ACCNEAT_SRC_UTIL_MAP_H_

#pragma once

#include <vector>

#include "population.h"
#include "speciesorganism.h"
#include "util/organismsbuffer.h"

namespace NEAT
{
  class SpeciesPopulation
          : public Population
  {
    public:
    /// \brief Construct off of a single spawning Genome
    SpeciesPopulation(rng_t rng,
                      std::vector<std::unique_ptr<Genome>> &seeds);

    /// \brief
    virtual ~SpeciesPopulation();

    /// \brief
    virtual size_t size() override;

    /// \brief
    virtual class Organism *get(size_t index) override;

    /// \brief
    virtual std::unique_ptr<Organism> make_copy(size_t index) override;

    /// \brief
    virtual void next_generation() override;

    /// \brief
    virtual void verify() override;

    //    virtual void
    //    write(std::ostream &out) override;

    private:
    /// \brief
    void spawn();

    /// \brief
    void speciate();

    /// \brief
    size_t norgs;

    /// \brief
    int generation;

    /// \brief
    OrganismsBuffer<SpeciesOrganism> orgs;

    /// \brief Species in the SpeciesPopulation. Note that the species should
    /// comprise all the genomes
    std::vector<class Species *> species;

    // ******* Member variables used during reproduction *******
    /// \brief The highest species number
    int last_species;

    // ******* When do we need to delta code? *******
    /// \brief Stagnation detector
    real_t highest_fitness;

    /// \brief If too high, leads to delta coding
    int highest_last_changed;
  };
}  // namespace NEAT

#endif
