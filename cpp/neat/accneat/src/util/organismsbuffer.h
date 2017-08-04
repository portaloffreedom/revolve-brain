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

#ifndef CPP_NEAT_ACCNEAT_SRC_UTIL_ORGANISMSBUFFER_H_
#define CPP_NEAT_ACCNEAT_SRC_UTIL_ORGANISMSBUFFER_H_

#pragma once

#include <assert.h>
#include <vector>

#include "organism.h"
#include "rng.h"

namespace NEAT
{
  template <typename TOrganism = Organism>
  class OrganismsBuffer
  {
    size_t _n;

    std::vector<TOrganism> _a;

    std::vector<TOrganism> _b;

    std::vector<TOrganism> *_curr;

    std::vector<TOrganism> *_prev;

    public:
    OrganismsBuffer(rng_t rng,
                    std::vector<std::unique_ptr<Genome>> &seeds,
                    size_t n,
                    size_t population_index = 0)
            : _n(n)
    {
      _a.reserve(n);
      _b.reserve(n);
      _curr = &_a;
      _prev = &_b;

      for (size_t i = 0; i < n; i++)
      {
        _a.emplace_back(*seeds[i + population_index]);
        size_t ipop = i + population_index;
        _a[i].population_index = ipop;
        _a[i].net->population_index = ipop;
        _a[i].genome->genome_id = ipop;
        _a[i].genome->rng.seed(rng.integer());
      }
      for (size_t i = 0; i < n; i++)
      {
        _b.emplace_back(*seeds[i + population_index]);
        size_t ipop = i + population_index;
        _b[i].population_index = ipop;
        _b[i].net->population_index = ipop;
        _b[i].genome->genome_id = ipop;
        _b[i].genome->rng.seed(rng.integer());
      }
    }

    void init_phenotypes()
    {
#pragma omp parallel for
      for (size_t i = 0; i < _n; i++)
      {
        Organism &org = curr()[i];
        org.genome->init_phenotype(*org.net);
      }
    }

    size_t size()
    {
      return _n;
    }

    std::vector<TOrganism> &curr()
    {
      return *_curr;
    }

    std::vector<TOrganism> &prev()
    {
      return *_prev;
    }

    void next_generation(int generation)
    {
      if (_curr == &_a)
      {
        _curr = &_b;
        _prev = &_a;
      }
      else
      {
        _curr = &_a;
        _prev = &_b;
      }

      assert(_curr->size() == _n);

      for (TOrganism &org: curr())
      {
        org.init(generation);
      }
    }
  };
}

#endif
