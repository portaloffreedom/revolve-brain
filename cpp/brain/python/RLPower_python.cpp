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

#include <string>

#include <boost/python/import.hpp>

#include "object.h"
#include "PythonListWrap.h"

#include "RLPower_python.h"

using namespace revolve::brain;

RLPower_python::RLPower_python(std::string robot_name,
                               const boost::python::object &conf,
                               revolve::brain::EvaluatorPtr evaluator,
                               unsigned int n_actuators,
                               unsigned int n_sensors)
        : RLPower::RLPower(robot_name,
                           create_config(conf),
                           evaluator,
                           n_actuators,
                           n_sensors)
{
}

template <typename T>
T
read_or_default(const boost::python::object &o,
                const char *key,
                T default_value)
{
  // Remember AttributeError for later comparison
  //     boost::python::object attributeError =
  //     boost::python::import("AttributeError");
  try {
    const boost::python::object &o2 = o.attr(key);

    boost::python::extract<T> extractor(o2);
    if (extractor.check())
      return extractor;
    else
      return default_value;
  } catch (const boost::python::error_already_set &) {
    // https://misspent.wordpress.com/2009/10/11/
    // boost-python-and-handling-python-exceptions/
    PyObject *e, *v, *t;
    PyErr_Fetch(&e, &v, &t);

    return default_value;
  }

  return default_value;
}

RLPower::Config
RLPower_python::create_config(const boost::python::object &conf)
{
  RLPower::Config config;

  config.algorithm_type = read_or_default<std::string>
          (conf, "algorithm_type", "A");
  config.evaluation_rate = read_or_default<unsigned int>
          (conf, "evaluation_rate", RLPower::EVALUATION_RATE);
  config.interpolation_spline_size = read_or_default<unsigned int>
          (conf, "interpolation_spline_size", RLPower::INTERPOLATION_CACHE_SIZE);
  config.max_evaluations = read_or_default<unsigned int>
          (conf, "max_evaluations", RLPower::MAX_EVALUATIONS);
  config.max_ranked_policies = read_or_default<unsigned int>
          (conf, "max_ranked_policies", RLPower::MAX_RANKED_POLICIES);
  config.noise_sigma = read_or_default<double>
          (conf, "noise_sigma", RLPower::SIGMA_START_VALUE);
  config.sigma_tau_correction = read_or_default<double>
          (conf, "sigma_tau_correction", RLPower::SIGMA_TAU_CORRECTION);
  config.source_y_size = read_or_default<unsigned int>
          (conf, "init_spline_size", RLPower::INITIAL_SPLINE_SIZE);
  config.update_step = read_or_default<unsigned int>
          (conf, "update_step", RLPower::UPDATE_STEP);

  return config;
}


void RLPower_python::update(boost::python::list &actuators,
                       const boost::python::list &sensors,
                       double t,
                       double step)
{
  python_list_wrap<ActuatorPtr> actuator_wrap(&actuators);
  python_list_wrap<SensorPtr> sensor_wrap(&sensors);

  RLPower::update
          <python_list_wrap<ActuatorPtr>, python_list_wrap<SensorPtr>>
          (actuator_wrap,
           sensor_wrap,
           t,
           step);
}
