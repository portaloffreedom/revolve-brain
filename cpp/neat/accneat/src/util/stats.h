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

#ifndef CPP_NEAT_ACCNEAT_SRC_UTIL_STATS_H_
#define CPP_NEAT_ACCNEAT_SRC_UTIL_STATS_H_

#pragma once

#include <algorithm>
#include <vector>
#include <iostream>

namespace NEAT
{
  template < typename T, typename U = T >
  U sum(const std::vector< T > &vec)
  {
    U result = 0;
    for (const T &x: vec)
    { result += (U)x; }
    return result;
  }

  template < typename T, typename U = double >
  U mean(const std::vector< T > &vec)
  {
    if (vec.empty())
    { return NAN; }
    return sum< T, U >(vec) / vec.size();
  }

  template < typename T >
  T min(const std::vector< T > &vec)
  {
    T minval = vec.front();
    for (size_t i = 1; i < vec.size(); i++)
    {
      if (vec[i] < minval)
      {
        minval = vec[i];
      }
    }
    return minval;
  }

  template < typename T >
  T max(const std::vector< T > &vec)
  {
    T maxval = vec.front();
    for (size_t i = 1; i < vec.size(); i++)
    {
      if (vec[i] > maxval)
      {
        maxval = vec[i];
      }
    }
    return maxval;
  }

  struct stats_t
  {
    size_t n;
    double min;
    double max;
    double mean;
  };

  inline std::ostream &operator<<(
          std::ostream &out,
          const stats_t &stats)
  {
    return out << "n=" << stats.n
               << ", min=" << stats.min
               << ", max=" << stats.max
               << ", mean=" << stats.mean;
  }

  template < typename T >
  stats_t stats(const std::vector< T > &vec)
  {
    stats_t result;

    result.n = vec.size();
    if (result.n > 0)
    {
      result.min = min(vec);
      result.max = max(vec);
      result.mean = mean< T, double >(vec);
    }
    else
    {
      result.min = result.max = result.mean = 0;
    }

    return result;
  }
}

#endif
