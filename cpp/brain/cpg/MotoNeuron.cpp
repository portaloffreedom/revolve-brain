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
// Created by matteo on 09/11/16.
//

#include "MotoNeuron.h"
#include <sstream>
#include <cmath>

using namespace revolve::brain::cpg;

MotoNeuron::MotoNeuron(real_t v_max)
    : v_max(v_max)
{}

MotoNeuron::~MotoNeuron()
{}

std::vector<real_t> MotoNeuron::update(std::vector<real_t> inputs, real_t delta_time)
{
    if (inputs.size() != 2) {
        std::stringstream ss;
        ss << "input size should be 2, instead is "
           << inputs.size();
        throw invalid_input_exception(ss.str());
    }

    return { output(inputs[0], inputs[1])};
}

real_t MotoNeuron::output(real_t pfe, real_t pff) const
{
    real_t result;

    result = -2 * (pfe - pff);
    result = 1 + std::exp(result/v_max);
    result = (2 / result) - 1;
    result *= v_max;

    return result;
}
