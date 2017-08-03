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

//
// Created by matteo on 10/11/16.
//

#ifndef REVOLVE_BRAIN_TESTCPGBRAIN_H
#define REVOLVE_BRAIN_TESTCPGBRAIN_H

#include "brain/CPGBrain.h"
#include <vector>
#include <string>

class TestCPGBrain : public revolve::brain::CPGBrain
{
public:
    TestCPGBrain(std::string robot_name,
                 revolve::brain::EvaluatorPtr evaluator,
                 unsigned int n_actuators,
                 unsigned int n_sensors);

    void test_update(const std::vector<revolve::brain::ActuatorPtr> &actuators,
                     const std::vector<revolve::brain::SensorPtr> &sensors,
                     double t, double step);
};


#endif //REVOLVE_BRAIN_TESTCPGBRAIN_H
