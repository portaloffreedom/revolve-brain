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

CPGBrain_python::CPGBrain_python(std::string robot_name,
                                 EvaluatorPtr evaluator,
                                 unsigned int n_actuators,
                                 unsigned int n_sensors)
        : CPGBrain(robot_name, evaluator, n_actuators, n_sensors)
{
}

void CPGBrain_python::update(boost::python::list &actuators,
                             const boost::python::list &sensors,
                             double t,
                             double step)
{
  python_list_wrap<ActuatorPtr> actuator_wrap(&actuators);
  python_list_wrap<SensorPtr> sensor_wrap(&sensors);

  CPGBrain::update
          <python_list_wrap<ActuatorPtr>, python_list_wrap<SensorPtr>>
          (actuator_wrap, sensor_wrap, t, step);
}


