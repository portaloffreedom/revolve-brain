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

#include "BaseLearner.h"

using namespace revolve::brain;

BaseLearner::BaseLearner(std::unique_ptr<BaseController> controller, const std::string &robot_name)
        :
        active_controller(std::move(controller)),
        robot_name(robot_name)
{
}

revolve::brain::BaseLearner::BaseLearner(const std::string &robot_name)
        :
        active_controller(),
        robot_name(robot_name)
{
}


BaseLearner::~BaseLearner()
{
}


BaseController *
BaseLearner::update(const std::vector<SensorPtr> &sensors,
                     double t,
                     double step)
{
  BaseController *controller_p = this->active_controller.get();
  assert(controller_p != nullptr);
  return controller_p;
}

