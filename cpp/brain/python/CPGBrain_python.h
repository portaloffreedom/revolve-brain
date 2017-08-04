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

#ifndef REVOLVE_BRAIN_CPGBRAIN_PYTHON_H
#define REVOLVE_BRAIN_CPGBRAIN_PYTHON_H

#include <string>
#include <vector>

#include <boost/python/list.hpp>

#include "brain/CPGBrain.h"

namespace revolve
{
  namespace brain
  {
    class CPGBrain_python
            : public revolve::brain::CPGBrain
    {
      public:
      /// \brief
      CPGBrain_python(std::string robot_name,
                      EvaluatorPtr evaluator,
                      unsigned int n_actuators,
                      unsigned int n_sensors);

      /// \brief
      void update(boost::python::list &actuators,
                  const boost::python::list &sensors,
                  double t,
                  double step);
    };
  }
}


#endif  // REVOLVE_BRAIN_CPGBRAIN_PYTHON_H
