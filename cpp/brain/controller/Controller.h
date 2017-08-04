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
* Description: Specifies a utility `Contoller` base class, which is supposed to
*              control the robot according to a given genome.
* Author: TODO <Add proper author>
*
*/

#ifndef REVOLVEBRAIN_BRAIN_CONTROLLER_CONTROLLER_H_
#define REVOLVEBRAIN_BRAIN_CONTROLLER_CONTROLLER_H_

#include <vector>

#include "brain/Actuator.h"
#include "brain/Sensor.h"

namespace revolve
{
  namespace brain
  {
    template <typename Phenotype>
    class Controller
    {
      public:

      virtual ~Controller()
      {}

      /// \brief Get method for the genome
      /// \return current genome
      virtual Phenotype getPhenotype() = 0;


      /// \brief Set method for the phenome
      /// \param newGenome: new genome to use instead of the old one
      virtual void setPhenotype(Phenotype phenotype) = 0;

      /// \brief Update step called for the controller.
      /// \param actuators List of actuators
      /// \param sensors List of sensors
      /// \param t Current simulation time
      /// \param step Actuation step size in seconds
      virtual void update(const std::vector<ActuatorPtr> &actuators,
                          const std::vector<SensorPtr> &sensors,
                          double t,
                          double step) = 0;
    };
  }
}

#endif  //  REVOLVEBRAIN_BRAIN_CONTROLLER_CONTROLLER_H_
