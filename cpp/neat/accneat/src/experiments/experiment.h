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

#ifndef CPP_NEAT_ACCNEAT_SRC_EXPERIMENTS_EXPERIMENT_H_
#define CPP_NEAT_ACCNEAT_SRC_EXPERIMENTS_EXPERIMENT_H_

#pragma once

#include <map>
#include <vector>
#include <string>

namespace NEAT
{
  class Experiment
  {
    public:
    static std::vector< std::string > get_names();

    static Experiment *get(const char *name);

    private:
    static std::map< std::string, Experiment * > *experiments;

    public:
    virtual ~Experiment();

    virtual void run(
            class rng_t &rng,
            int gens) = 0;

    protected:
    Experiment(const char *name);

    const char *const get_name() const
    {
      return name;
    }

    private:
    Experiment()
    {}

    const char *name;
  };
}

#endif
