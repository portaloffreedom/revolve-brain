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

#include "FakeLightSensor.h"
#include <cmath>

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

double revolve::brain::FakeLightSensor::light_attenuation(double distance, float angle)
{
    float abs_angle = std::fabs(angle);

    if (abs_angle > half_fov)
        return 0;

    //TODO noise

    #define CONST_E 2.718281828459045235360287471352662497757247093699959574966
    // gaussian curve
    #define AVERAGE 0
    #define VARIANCE 0.25
    // distance = 0 => value = 1
    // distance = 0.5 => value = 0.2236..
    return std::pow(CONST_E, -((((distance-AVERAGE)*(distance-AVERAGE))/(2*VARIANCE))));
    #undef CONST_E
    #undef AVERAGE
    #undef VARIANCE
}
