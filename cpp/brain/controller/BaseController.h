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

#ifndef REVOLVE_BRAIN_BASECONTROLLER_H_
#define REVOLVE_BRAIN_BASECONTROLLER_H_

#include <vector>
#include "brain/Actuator.h"
#include "brain/Sensor.h"

namespace revolve
{
  namespace brain
  {
    class BaseController
    {
      public:
      /// \brief REMEMBER TO MAKE YOUR CHILD DECONSTRUCTORS VIRTUAL AS WELL
      virtual ~BaseController()
      {}

      /// \brief Update the controller to the next step and sends signals
      /// to the actuators
      ///
      /// \param actuators outputs of the controller
      /// \param sensors inputs of the controller
      /// \param t global time reference
      /// \param step time since last update
      virtual void update(
              const std::vector< ActuatorPtr > &actuators,
              const std::vector< SensorPtr > &sensors,
              double t,
              double step) = 0;
    };
  }
}

#endif  //  REVOLVE_BRAIN_BASECONTROLLER_H_
