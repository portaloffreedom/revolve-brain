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

#include <vector>

#include "GenericLearnerBrain.h"

using namespace revolve::brain;

GenericLearnerBrain::GenericLearnerBrain(std::unique_ptr<BaseLearner> learner)
    : learner(std::move(learner))
{
}

GenericLearnerBrain::GenericLearnerBrain(BaseLearner *learner)
    : learner(learner)
{
}

void GenericLearnerBrain::update(const std::vector<ActuatorPtr> &actuators,
                                 const std::vector<SensorPtr> &sensors,
                                 double t, double step)
{
  BaseController *controller = learner->update(sensors, t, step);
  controller->update(actuators, sensors, t, step);
}
