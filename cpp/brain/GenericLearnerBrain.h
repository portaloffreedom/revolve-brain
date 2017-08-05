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
* Date: March 15, 2017
*
*/

#ifndef REVOLVE_BRAIN_GENERICLEARNERBRAIN_H
#define REVOLVE_BRAIN_GENERICLEARNERBRAIN_H

#include <vector>

#include "Brain.h"
#include "brain/learner/BaseLearner.h"

namespace revolve
{
  namespace brain
  {
    class GenericLearnerBrain
            : public Brain
    {
      public:
      using Brain::update;

      /// \brief
      GenericLearnerBrain(std::unique_ptr<BaseLearner> learner);

      /// \brief
      GenericLearnerBrain(BaseLearner *learner);

      /// \brief
      void update(const std::vector<ActuatorPtr> &actuators,
                  const std::vector<SensorPtr> &sensors,
                  double t,
                  double step) override;

      private:
      /// \brief
      std::unique_ptr<BaseLearner> learner;
    };
  }
}


#endif  // REVOLVE_BRAIN_GENERICLEARNERBRAIN_H}}

