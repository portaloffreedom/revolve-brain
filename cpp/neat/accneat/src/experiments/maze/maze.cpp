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

#include "experiments/evaluatorexperiment.h"
#include "mazeevaluator.h"

using namespace NEAT;

static struct MazeInit
{
    MazeInit()
    {
      auto create_evaluator = []()
      {
          return create_maze_evaluator();
      };

      auto create_seeds = [](rng_t rng_exp)
      {
          return env->genome_manager->create_seed_generation(
                  env->pop_size,
                  rng_exp,
                  1,
                  __sensor_N,
                  __output_N,
                  __sensor_N,
                  "maze_experiment");
      };

      // TODO: Should maybe make an explicit static registry func?
      new EvaluatorExperiment("maze", create_evaluator, create_seeds);
    }
} init;
