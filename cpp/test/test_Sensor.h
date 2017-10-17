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

#ifndef TESTSENSOR_H
#define TESTSENSOR_H

#include <random>
#include <string>

#include "brain/Sensor.h"

class TestSensor
        : public revolve::brain::Sensor
{
  public:
  TestSensor(bool verbose = false);

  void read(double *input_vector) override;

  size_t inputs() const override;

  std::string sensorId() const override;

  private:
  bool verbose;

  std::random_device rd;

  std::mt19937 gen;

  std::uniform_real_distribution<double> dis;
};

#endif  // TESTSENSOR_H
