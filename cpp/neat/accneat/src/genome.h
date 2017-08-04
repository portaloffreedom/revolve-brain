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

#ifndef CPP_NEAT_ACCNEAT_SRC_GENOME_H_
#define CPP_NEAT_ACCNEAT_SRC_GENOME_H_

#pragma once

#include <string>

#include "util/rng.h"

namespace NEAT
{
  class Genome
  {
    public:
    /// \brief
    rng_t rng;

    /// \brief
    int genome_id;

    /// \brief
    const std::string *robot_name;

    /// \brief
    Genome(const std::string &robot_name)
            : Genome(&robot_name)
    {}

    /// \brief
    Genome(const std::string *robot_name)
            : robot_name(robot_name)
    {}

    /// \brief
    virtual ~Genome()
    {}

    /// \brief
    virtual Genome &operator=(const Genome &other) = 0;

    /// \brief
    virtual void init_phenotype(class Network &net) = 0;

    /// \brief
    virtual void verify() = 0;

    /// \brief
    virtual void save(std::ostream &out) const = 0;

    /// \brief
    virtual bool load(std::istream &in) = 0;

    /// \brief
    struct Stats
    {
      size_t nnodes;
      size_t nlinks;
    };

    /// \brief
    virtual Stats get_stats() = 0;
  };
}  // namespace NEAT

#endif
