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

#ifndef REVOLVEBRAIN_BRAIN_PYTHON_SENSORWRAP_H_
#define REVOLVEBRAIN_BRAIN_PYTHON_SENSORWRAP_H_

#include <string>

#include <boost/python/wrapper.hpp>
#include <boost/python/object.hpp>

#include "brain/Sensor.h"
#include "brain/python/PythonArray.h"

namespace revolve
{
  namespace brain
  {
    /// \brief Class needed to enable calling virtual methods implemented child
    /// python classes
    class SensorWrap
            : public Sensor
              , public boost::python::wrapper<Sensor>
    {
      public:
      virtual void read(double *input_vector)
      {
        this->get_override("read")(python_array<double>(input_vector));
      }

      virtual size_t inputs() const
      {
        return this->get_override("inputs")();
      };

      virtual std::string sensorId() const
      {
        return this->get_override("sensorId")();
      };
    };
  }
}

#endif  //  REVOLVEBRAIN_BRAIN_PYTHON_SENSORWRAP_H_
