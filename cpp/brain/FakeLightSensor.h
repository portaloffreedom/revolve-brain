/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2016  Matteo De Carlo <matteo.dek@covolunablu.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef REVOLVE_BRAIN_FAKELIGHT_H
#define REVOLVE_BRAIN_FAKELIGHT_H

#include "Sensor.h"
#include <iostream>

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
        // std::cout << "revolve::brain::FakeLightSensor()" << std::endl;
        verbose = std::getenv("FAKE_LIGHT_SENSOR_VERBOSE") != nullptr;
    }

    virtual double read();
    virtual void read(double* input_vector) override;
    virtual unsigned int inputs() const override;

    virtual double light_distance() = 0;

    virtual void replace(const FakeLightSensor *const new_sensor);
    virtual void replace(const boost::shared_ptr<FakeLightSensor> &new_sensor) {
        this->replace(new_sensor.get());
    }
protected:
    virtual double light_angle() = 0;

    virtual double light_attenuation(double distance, float angle);

    float half_fov;
    bool verbose;
};

}
}

#endif // REVOLVE_BRAIN_FAKELIGHT_H
