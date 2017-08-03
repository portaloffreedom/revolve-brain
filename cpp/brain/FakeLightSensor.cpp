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
*
*/

#include <cmath>

#include "FakeLightSensor.h"

using namespace revolve::brain;

unsigned int FakeLightSensor::inputs() const
{
    return 1;
}

double revolve::brain::FakeLightSensor::read()
{
    return light_attenuation(light_distance(), light_angle());
}


void FakeLightSensor::read(double* input_vector)
{
    input_vector[0] = read();
}

float revolve::brain::FakeLightSensor::light_attenuation(float distance, float angle)
{
    float abs_angle = std::fabs(angle);

    if (abs_angle > half_fov)
        return 0;

    //TODO noise
    return distance;
}
