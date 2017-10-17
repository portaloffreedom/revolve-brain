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

#ifndef REVOLVEBRAIN_BRAIN_PYTHON_ACTUATORWRAP_H_
#define REVOLVEBRAIN_BRAIN_PYTHON_ACTUATORWRAP_H_

#include <iostream>

#include <boost/python/object.hpp>
#include <boost/python/wrapper.hpp>

#include "brain/Actuator.h"
#include "brain/python/PythonArray.h"

namespace revolve
{
  namespace brain
  {
    /// \brief Class needed to enable calling virtual methods implemented child
    /// python classes
    class ActuatorWrap
            : public revolve::brain::Actuator
              , public boost::python::wrapper<Actuator>
    {
      public:
      virtual void
      update(double *output_vector,
             double step)
      {
        this->get_override("update")(python_array<double>(output_vector),
                                     step);
      }

      virtual size_t outputs() const
      {
        return this->get_override("outputs")();
      };
    };
  }
}

#endif  //  REVOLVEBRAIN_BRAIN_PYTHON_ACTUATORWRAP_H_
