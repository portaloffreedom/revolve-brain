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

#include <iostream>
#include <vector>

#include "SUPGNeuron.h"

#define NCYCLES 1

size_t SUPGNeuron::GetDimensionInput(size_t n_inputs,
                                           size_t coordinates_size)
{
  return 1 + coordinates_size + n_inputs;
}

size_t SUPGNeuron::GetDimensionOutput(size_t n_outputs)
{
  return 2 + n_outputs;
}

SUPGNeuron::SUPGNeuron(NEAT::CpuNetwork *cppn,
                       std::vector<float> coordinates,
                       NEAT::real_t cicle_length)
        : cppn(cppn)
          , start_timer(-1)
          , timer_window(cicle_length)
          , started_timer_flag(false)
          , supg_internal_inputs(GetDimensionInput(0, coordinates.size()))
          , supg_internal_outputs(GetDimensionOutput(0))
{
  set_coordinates(coordinates);
  NEAT::NetDims dims = cppn->get_dims();
  size_t n_input = dims.nnodes.input;
  size_t n_output = dims.nnodes.output;

  if (n_input < supg_internal_inputs)
  {
    std::stringstream ss;
    ss << "SUPGNeuron::SUPGNeuron() Invalid network input size or coordinate "
            "vector size (cppn.input_size == "
       << n_input << " | coordinates.size() == " << coordinates.size() << " )";
    throw std::invalid_argument(ss.str());
  }
  if (n_output < supg_internal_outputs)
  {
    std::stringstream ss;
    ss << "SUPGNeuron::SUPGNeuron() Invalid network output size "
            "(cppn.output_size == " << n_output << " )";
    throw std::invalid_argument(ss.str());
  }
}

SUPGNeuron::SUPGNeuron(const SUPGNeuron &other,
                       std::vector<float> coordinates,
                       NEAT::real_t cicle_length)
        : SUPGNeuron(other.cppn, coordinates, cicle_length)
{}

SUPGNeuron::~SUPGNeuron()
{
}

NEAT::NetDims SUPGNeuron::get_dims()
{
  NEAT::NetDims dims = cppn->get_dims();
  dims.nnodes.input -= supg_internal_inputs;
  dims.nnodes.output -= supg_internal_outputs;
  return dims;
}

void SUPGNeuron::setCppn(NEAT::CpuNetwork *cppn)
{
  NEAT::NetDims prev_dims = this->cppn->get_dims();
  NEAT::NetDims next_dims = cppn->get_dims();

  if ((prev_dims.nnodes.input not_eq next_dims.nnodes.input)
      or (prev_dims.nnodes.output not_eq next_dims.nnodes.output))
  {
    throw std::invalid_argument("SUPGNeuron::setCppn() cppn "
                                        "dimensions do not correspond");
  }

  this->cppn = cppn;
}

void SUPGNeuron::reset(float global_time)
{
  start_timer = global_time;
}

// create value from cppn (limit value from 0 to 1)
void SUPGNeuron::init_timer(float global_time)
{
  cppn->load_sensor(Input::TIMER, 0);
  load_coordinates();

  // reset all other inputs
  size_t n_other_inputs = cppn->get_dims().nnodes.input
                                - supg_internal_inputs;
  for (size_t i = 0; i < n_other_inputs; i++)
  {
    this->load_sensor(i, 0);
  }

  cppn->activate(NCYCLES);

  NEAT::real_t *outputs = cppn->Outputs();
  NEAT::real_t offset = outputs[Output::OFFSET];

  if (offset < 0 or offset > 1)
  {
    offset = 0;
  }

  offset *= timer_window;

  reset(global_time);
  start_timer -= offset;
}

void SUPGNeuron::set_coordinates(std::vector<float> coordinates)
{
  this->coordinates = coordinates;
}

void SUPGNeuron::load_coordinates()
{
  for (size_t i = 0; i < coordinates.size(); i++)
  {
    cppn->load_sensor(
            Input::COORDINATE_OFFSET + i,
            coordinates[i]);
  }
}

NEAT::real_t SUPGNeuron::get_timer(float global_time)
{
  //     if (coordinates[0] == 0)
  //     std::cout << "#### (" << coordinates[0] << ", "
  //               << coordinates[1] << ") ####" << std::endl;
  if (not started_timer_flag)
  {
    init_timer(global_time);
    started_timer_flag = true;
  }

  NEAT::real_t timer = (global_time - start_timer) / timer_window;

  // this difference should never be negative
  if (timer < 0)
  {
    std::cerr
            << "SUPGNeuron::get_timer() error, negative timer value"
            << std::endl;
    timer = 0;
  }

  // reset timer if value one was passed
  if (timer > 1)
  {
    NEAT::real_t int_part;
    timer = std::modf(timer,
                      &int_part);
    start_timer += (int_part * timer_window);
    //         if (coordinates[0] == 0)
    //             std::cout << "CIRCL! " << int_part << std::endl;
  }

  //     if (coordinates[0] == 0)
  //         std::cout << '[' << global_time << " - "
  //          << start_timer << "] timer: " << timer << std::endl;
  // std::cout << timer << std::endl;

  //     if (coordinates[0] == 0)
  //         std::cout << "#### (" << coordinates[0]
  //         << ", " << coordinates[1] << ") ####" << std::endl;
  return timer;
}

void SUPGNeuron::load_sensor(size_t isensor,
                             NEAT::real_t activation)
{
  cppn->load_sensor(isensor + supg_internal_inputs, activation);
}

void SUPGNeuron::activate(float global_time)
{
  NEAT::real_t timer = get_timer(global_time);

  cppn->load_sensor(Input::TIMER, timer);
  load_coordinates();
  cppn->activate(NCYCLES);

  NEAT::real_t *outputs = cppn->Outputs();
  NEAT::real_t offset = outputs[Output::RESET_TRIGGER];
  if (offset > 0.9)
  {
    reset(global_time);
  }
}

NEAT::real_t *SUPGNeuron::get_outputs()
{
  NEAT::real_t *outputs = cppn->Outputs();
  return outputs + supg_internal_outputs;
}
