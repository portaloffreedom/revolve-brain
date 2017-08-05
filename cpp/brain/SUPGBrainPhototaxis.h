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

#ifndef REVOLVE_BRAIN_SUPGBRAINPHOTOTAXIS_H
#define REVOLVE_BRAIN_SUPGBRAINPHOTOTAXIS_H

#include <string>
#include <vector>

#include "FakeLightSensor.h"
#include "SUPGBrain.h"

namespace revolve
{
  namespace brain
  {
    class SUPGBrainPhototaxis
            : protected SUPGBrain
    {
      public:
      /// \brief Light sensor constructors are passed the coordinates with
      /// already the offset included
      SUPGBrainPhototaxis(
              const std::string &robot_name,
              EvaluatorPtr evaluator,
              std::function
                      < boost::shared_ptr
                                < FakeLightSensor >
                                (std::vector< float > coordinates) >
              _light_constructor_left,
              std::function
                      < boost::shared_ptr
                                < FakeLightSensor >
                                (std::vector< float > coordinates) >
              _light_constructor_right,
              double light_radius_distance,
              const std::vector< std::vector< float > > &neuron_coordinates,
              const std::vector< ActuatorPtr > &actuators,
              const std::vector< SensorPtr > &sensors);

      using SUPGBrain::update;

      virtual void update(const std::vector< ActuatorPtr > &actuators,
                          const std::vector< SensorPtr > &sensors,
                          double t,
                          double step) override;

      protected:
      SUPGBrainPhototaxis(EvaluatorPtr evaluator);

      virtual double getFitness() override;

      virtual double getPhaseFitness();

      virtual void learner(double t) override;

      //// Templates ---------------------------------------------------------

      enum PHASE
      {
        CENTER = 0,
        LEFT = 1,
        MORELEFT = 2,
        RIGHT = 3,
        MORERIGHT = 4,
        END = 5,
      } phase;

      std::function< boost::shared_ptr< FakeLightSensor >(
              std::vector< float > coordinates) > light_constructor_left;

      std::function< boost::shared_ptr< FakeLightSensor >(
              std::vector< float > coordinates) > light_constructor_right;

      boost::shared_ptr< FakeLightSensor > current_light_left;

      boost::shared_ptr< FakeLightSensor > current_light_right;

      double light_radius_distance;

      double partial_fitness;
    };
  }
}

#endif  //  REVOLVE_BRAIN_SUPGBRAINPHOTOTAXIS_H
