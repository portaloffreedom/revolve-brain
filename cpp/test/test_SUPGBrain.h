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

#ifndef TESTSUPGBRAIN_H
#define TESTSUPGBRAIN_H

#include <string>
#include <vector>

#include "brain/SUPGBrain.h"

class TestSUPGBrain
        : public revolve::brain::SUPGBrain
{
  public:
  TestSUPGBrain(
          revolve::brain::EvaluatorPtr evaluator,
          const std::vector< std::vector< float > > &neuron_coordinates,
          const std::vector< revolve::brain::ActuatorPtr > &actuators,
          const std::vector< revolve::brain::SensorPtr > &sensors);


  void test_update(
          const std::vector< revolve::brain::ActuatorPtr > &actuators,
          const std::vector< revolve::brain::SensorPtr > &sensors,
          double t,
          double step);
};

#endif  // TESTSUPGBRAIN_H
