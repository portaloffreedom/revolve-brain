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
* Description: Specifies a utility `Brain` base class. If your brain doesn't
*              fit this model, something else can easily be used by simply
*              ignoring the default brain behavior in the `RobotController`.
* Author: Elte Hupkes
*
*/

#ifndef REVOLVEBRAIN_BRAIN_BRAIN_H_
#define REVOLVEBRAIN_BRAIN_BRAIN_H_

#include <vector>

#include "Actuator.h"
#include "Sensor.h"

namespace revolve
{
  namespace brain
  {
    class Brain
    {
      public:
      Brain()
              : is_offline_(false)
      {}

      virtual ~Brain()
      {}

      /// \brief Update step called for the brain.
      /// \param actuators List of actuators
      /// \param sensors List of sensors
      /// \param t Current simulation time
      /// \param step Actuation step size in seconds
      virtual void update(
              const std::vector< ActuatorPtr > &actuators,
              const std::vector< SensorPtr > &sensors,
              double t,
              double step) = 0;


      /// \brief If offline, the robot won't update his controller. This is
      /// delegated to a supervisor.
      /// \return true if the brain is offline
      bool isOffline() const
      {
        return this->is_offline_;
      }

      /// \brief If offline, the robot won't update his controller. This is
      /// delegated to a supervisor
      /// \param offline set offline mode on or off
      virtual void setOffline(const bool is_offline)
      {
        this->is_offline_ = is_offline;
      }

      /// \brief
      protected:
      bool is_offline_;
    };
  }
}

#endif  //  REVOLVEBRAIN_BRAIN_BRAIN_H_
