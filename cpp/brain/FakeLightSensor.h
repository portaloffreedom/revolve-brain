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

#ifndef REVOLVE_BRAIN_FAKELIGHT_H
#define REVOLVE_BRAIN_FAKELIGHT_H

#include <iostream>

#include "Sensor.h"

namespace revolve {
namespace brain {

class FakeLightSensor : public revolve::brain::Sensor
{
public:
    // disable default constructor
    FakeLightSensor() = delete;

    explicit FakeLightSensor(float fov)
        : half_fov(fov/2)
        // TODO noise
    {
//         std::cout << "revolve::brain::FakeLightSensor()" << std::endl;
    }

    virtual double read();
    virtual void read(double* input_vector) override;
    virtual unsigned int inputs() const override;

protected:
    virtual double light_distance() = 0;
    virtual double light_angle() = 0;

    virtual float light_attenuation(float distance, float angle);

    float half_fov;
};

}
}

#endif  //  REVOLVE_BRAIN_FAKELIGHT_H
