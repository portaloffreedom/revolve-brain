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

#include "test_Sensor.h"

TestSensor::TestSensor(bool verbose)
        : verbose(verbose)
        , rd()
        , gen(rd())
        , dis(0, 2)
{
}

std::string TestSensor::sensorId() const
{
  return std::string("TestSensor");
}

unsigned int TestSensor::inputs() const
{
  return 1;
}

void TestSensor::read(double *input_vector)
{
  input_vector[0] = dis(gen);

  if (verbose)
    std::cout << "TestSensor::read <- " << input_vector[0] << std::endl;
}
