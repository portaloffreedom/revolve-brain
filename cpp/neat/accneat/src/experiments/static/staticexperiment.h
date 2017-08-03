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

#include "experiments/evaluatorexperiment.h"
#include "staticevaluator.h"

namespace NEAT {

typedef std::function<std::vector<Test>()> GetStaticTestsFunc;

inline void
create_static_experiment(const char *name,
                         GetStaticTestsFunc get_tests)
{

  auto create_evaluator =
          [get_tests]()
          {
              return create_static_evaluator(get_tests());
          };

  auto create_seeds = [get_tests](rng_t rng_exp)
  {
      Step s = get_tests().front().steps.front();

      return
              env->genome_manager->create_seed_generation(env->pop_size,
                                                          rng_exp,
                                                          1,
                                                          s.input.size(),
                                                          s.output.size(),
                                                          s.input.size(),
                                                          "static_experiment");
  };

  //todo: This is wonky. Should maybe make an explicit static registry func?
  new EvaluatorExperiment(name,
                          create_evaluator,
                          create_seeds);
}

}
