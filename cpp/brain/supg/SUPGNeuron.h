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

#ifndef REVOLVEBRAIN_BRAIN_SUPGG_SUPGNEURON_H_
#define REVOLVEBRAIN_BRAIN_SUPGG_SUPGNEURON_H_

#include <vector>
#include <chrono>

#include "network/cpu/cpunetwork.h"

class SUPGNeuron
{
  public:
  /// \brief
  SUPGNeuron(
          NEAT::CpuNetwork *cppn,
          std::vector< float > coordinates,
          NEAT::real_t cicle_length);

  /// \brief
  SUPGNeuron(
          const SUPGNeuron &other,
          std::vector< float > coordinates,
          NEAT::real_t cicle_length);

  /// \brief
  virtual ~SUPGNeuron();

  /// \brief
  static size_t GetDimensionInput(
          size_t n_inputs,
          size_t coordinates_size);

  /// \brief
  static size_t GetDimensionOutput(size_t n_outputs);

  /// \brief
  void reset(float global_time);

  /// \brief
  void activate(float global_time);

  /// \brief
  void load_sensor(
          size_t isensor,
          NEAT::real_t activation);

  /// \brief
  void setCppn(NEAT::CpuNetwork *cppn);

  /// \brief
  NEAT::real_t *get_outputs();

  /// \brief
  NEAT::NetDims get_dims();

  private:
  /// \brief
  NEAT::CpuNetwork *cppn;

  /// \brief
  NEAT::real_t start_timer;

  /// \brief
  std::vector< float > coordinates;

  /// \brief
  NEAT::real_t const timer_window;

  /// \brief
  bool started_timer_flag;

  /// \brief create values from cppn (limit value from 0 to 1)
  void init_timer(float global_time);

  /// \brief
  void set_coordinates(std::vector< float > coordinates);

  /// \brief
  void load_coordinates();

  /// \brief
  NEAT::real_t Timer(float global_time);

  public:
  enum Input
  {
    TIMER = 0,
    COORDINATE_OFFSET = 1
  };

  /// \brief 1 + coordinate.size()
  const size_t supg_internal_inputs;

  enum Output
  {
    OFFSET = 0,
    RESET_TRIGGER = 1
  };

  /// \brief 2
  const size_t supg_internal_outputs;
};

#endif  //  REVOLVEBRAIN_BRAIN_SUPGG_SUPGNEURON_H_
