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

#ifndef CPP_NEAT_ACCNEAT_SRC_EXPERIMENTS_MAZE_MAZEEVALUATOR_H_
#define CPP_NEAT_ACCNEAT_SRC_EXPERIMENTS_MAZE_MAZEEVALUATOR_H_

#pragma once

namespace NEAT
{
  enum sensor_t
  {
    sensor_right = 0,
    sensor_fwd = 1,
    sensor_left = 2,
    sensor_sound = 3,
    sensor_freq = 4,
    sensor_go = 5,
    __sensor_N = 6
  };

  enum output_t
  {
    output_right = 0,
    output_left = 1,
    output_fwd = 2,
    __output_N = 3
  };

  class NetworkEvaluator *create_maze_evaluator();
}

#endif
