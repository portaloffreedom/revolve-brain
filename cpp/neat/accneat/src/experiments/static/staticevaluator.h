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

#ifndef CPP_NEAT_ACCNEAT_SRC_EXPERIMENTS_STATIC_STATICEVALUATOR_H_
#define CPP_NEAT_ACCNEAT_SRC_EXPERIMENTS_STATIC_STATICEVALUATOR_H_

#pragma once

#include <string>
#include <vector>

#include "neattypes.h"

namespace NEAT
{
  /// \brief Specifies a set of input activations and an expected set of output
  /// activations.
  struct Step
  {
    std::vector<real_t> input;
    std::vector<real_t> output;
    real_t weight;

    Step(const std::vector<real_t> &input_,
         const std::vector<real_t> &output_,
         real_t weight_ = 1.0)
            : input(input_)
              , output(output_)
              , weight(weight_)
    {
    }
  };

  /// \brief A set of Steps for which the neural net state is expected to begin
  /// in its default state.
  struct Test
  {
    std::string name;
    std::vector<Step> steps;

    Test(const std::string &name_,
         const std::vector<Step> &steps_)
            : name(name_)
              , steps(steps_)
    {
    }

    Test(const std::vector<Step> &steps_)
            : name("")
              , steps(steps_)
    {
    }
  };

  extern class NetworkEvaluator *
  create_static_evaluator(const std::vector<Test> &tests);
}

#endif
