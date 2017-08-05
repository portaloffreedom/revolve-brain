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
* Author: Matteo De Carlo
* Date: November 14, 2016
*
*/

#include <string>
#include <vector>

#include <boost/make_shared.hpp>

#include "test_Evaluator.h"
#include "test_Actuator.h"
#include "test_Sensor.h"

#include "test_SUPGBrain.h"

const std::string test_name = "TestSUPGBrain";

int main()
{
  std::cout << "testing supgbrain" << std::endl;
  AsyncNeat::Init(test_name);

  revolve::brain::EvaluatorPtr testEvaluator =
          boost::make_shared< TestEvaluator >();

  std::vector< revolve::brain::SensorPtr > sensors;
  for (int i = 0; i < 10; i++)
  {
    sensors.push_back(boost::make_shared< TestSensor >());
  }

  std::vector< std::vector< float > > neuron_coordinates;
  std::vector< revolve::brain::ActuatorPtr > actuators;
  for (int i = 0; i < 10; i++)
  {
    actuators.push_back(boost::make_shared< TestActuator >());
    neuron_coordinates.push_back({-i * 1.0f, i * 1.0f});
  }

  TestSUPGBrain testBrain(
          testEvaluator,
          neuron_coordinates,
          actuators,
          sensors);

  float delta_time = 0.1;

  for (float time = 0; time < 1000; time += delta_time)
  {
    testBrain.test_update(actuators, sensors, time, delta_time);
  }

  AsyncNeat::CleanUp();
}

TestSUPGBrain::TestSUPGBrain(
        revolve::brain::EvaluatorPtr evaluator,
        const std::vector< std::vector< float > > &neuron_coordinates,
        const std::vector< revolve::brain::ActuatorPtr > &actuators,
        const std::vector< revolve::brain::SensorPtr > &sensors)
        : revolve::brain::SUPGBrain(test_name,
                                    evaluator,
                                    neuron_coordinates,
                                    actuators,
                                    sensors)
{
}

void TestSUPGBrain::test_update(
        const std::vector< revolve::brain::ActuatorPtr > &actuators,
        const std::vector< revolve::brain::SensorPtr > &sensors,
        double t,
        double step)
{
  this->update(actuators, sensors, t, step);
}
