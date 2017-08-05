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

#ifndef CPP_NEAT_ACCNEAT_SRC_SPECIES_SPECIESORGANISM_H_
#define CPP_NEAT_ACCNEAT_SRC_SPECIES_SPECIESORGANISM_H_

#pragma once

#include "organism.h"

namespace NEAT
{
  class SpeciesOrganism
          : public Organism
  {
    public:
    /// \brief The Organism's Species
    class Species *species;

    /// \brief
    real_t adjusted_fitness;

    /// \brief Number of children this Organism may have
    real_t expected_offspring;

    /// \brief Marker for destruction of inferior Organisms
    bool eliminate;

    /// \brief Marks the species champ
    bool champion;

    /// \brief Number of reserved offspring for a population leader
    int super_champ_offspring;

    /// \brief
    SpeciesOrganism(const SpeciesOrganism &other);

    /// \brief
    SpeciesOrganism(const Genome &genome);

    /// \brief
    virtual ~SpeciesOrganism();

    /// \brief
    virtual void init(int gen) override;

    protected:
    /// \brief
    virtual void copy_into(Organism &dst) const override;
  };
}

#endif
