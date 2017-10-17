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
* Date: September 11, 2016
*
*/

#include <string>

#include "PythonListWrap.h"

#include "CPGBrain_python.h"

using namespace revolve::brain;

CPGBrain_python::CPGBrain_python(
        const std::string &_name,
        EvaluatorPtr evaluator,
        const size_t n_actuators,
        const size_t n_sensors
)
        : CPGBrain(_name, evaluator, n_actuators, n_sensors)
{
}

void CPGBrain_python::update(
        boost::python::list &_actuators,
        const boost::python::list &_sensors,
        double t,
        double step)
{
  python_list_wrap< ActuatorPtr > actuator_wrap(&_actuators);
  python_list_wrap< SensorPtr > sensor_wrap(&_sensors);

  CPGBrain::update
          < python_list_wrap< ActuatorPtr >, python_list_wrap< SensorPtr>>
          (actuator_wrap, sensor_wrap, t, step);
}


