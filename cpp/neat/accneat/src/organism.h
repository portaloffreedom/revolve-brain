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

#ifndef CPP_NEAT_ACCNEAT_SRC_ORGANISM_H_
#define CPP_NEAT_ACCNEAT_SRC_ORGANISM_H_

#pragma once

#include <memory>

#include "genome.h"
#include "network/network.h"

namespace NEAT
{
  /// \brief ORGANISM CLASS:
  ///   Organisms are Genomes and Networks with fitness
  ///   information
  ///   i.e. The genotype and phenotype together
  class Organism
  {
    public:
    /// \brief Unique within the population,always in [0, env->pop_size).
    size_t population_index;

    /// \briefProvides client with convenient storage of associated
    /// data in an array.
    OrganismEvaluation eval;

    /// \brief The Organism's genotype
    std::unique_ptr< Genome > genome;

    /// \brief The Organism's phenotype
    std::unique_ptr< Network > net;

    /// \brief Tells which generation this Organism is from
    int generation;

    /// \brief
    Organism(const Organism &other);

    /// \brief
    Organism(const Genome &genome);

    /// \brief
    virtual ~Organism();

    /// \brief
    virtual void init(int gen);

    /// \brief
    Organism &operator=(const Organism &other);

    //    virtual void
    //    write(std::ostream &out) const;
    protected:
    /// \brief
    Organism()
    {}

    /// \brief
    virtual void copy_into(Organism &dst) const;
  };
}   // namespace NEAT

#endif
