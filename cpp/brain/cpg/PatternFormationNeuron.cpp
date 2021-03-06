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
* Date: November 8, 2016
*
*/

#include <cmath>
#include <vector>

#include "PatternFormationNeuron.h"

using namespace revolve::brain::cpg;

PatternFormationNeuron::PatternFormationNeuron(
        real_t alpha,
        real_t theta,
        std::vector< real_t > weights)
{
  setAlpha(alpha);
  setTheta(theta);
  setWeights(weights);
}

PatternFormationNeuron::~PatternFormationNeuron()
{}

std::vector< real_t > PatternFormationNeuron::update(
        std::vector< real_t > inputs,
        real_t /*delta_time*/)
{
  real_t combined_inputs = generateInput(inputs);
  real_t result = output(combined_inputs);

  return {result};
}

real_t PatternFormationNeuron::generateInput(std::vector< real_t > inputs) const
{
  if (inputs.size() not_eq weights.size())
  {
    std::stringstream ss;
    ss << "input size should be the same as weight("
       << weights.size()
       << "), instead is "
       << inputs.size();
    throw invalid_input_exception(ss.str());
  }

  real_t combined_inputs = 0;
  size_t input_size = weights.size();
  for (size_t i = 0; i < input_size; ++i)
  {
    combined_inputs += weights[i] * inputs[i];
  }

  return combined_inputs / input_size;
}

real_t PatternFormationNeuron::output(real_t combined_inputs) const
{
  real_t result;

  result = (theta * combined_inputs) - combined_inputs;
  result = 1 + alpha * std::exp(result);
  result = 1 / result;

  return result;
}

real_t PatternFormationNeuron::Alpha() const
{
  return alpha;
}

void PatternFormationNeuron::setAlpha(real_t alpha)
{
  PatternFormationNeuron::alpha = alpha;
}

real_t PatternFormationNeuron::Theta() const
{
  return theta;
}

void PatternFormationNeuron::setTheta(real_t theta)
{
  PatternFormationNeuron::theta = theta;
}

const std::vector< real_t > &PatternFormationNeuron::Weights() const
{
  return weights;
}

void PatternFormationNeuron::setWeights(const std::vector< real_t > &weights)
{
  PatternFormationNeuron::weights = std::vector< real_t >(weights);

  // Weight normalization
  real_t weight_sum = 0;
  size_t weights_size = this->weights.size();
  for (size_t i = 0; i < weights_size; ++i)
  {
    weight_sum += this->weights[i];
  }
  for (size_t i = 0; i < weights_size; ++i)
  {
    this->weights[i] /= weight_sum;
  }
}

// FROM PERCENTAGE ------------------------------------------------------------
real_t PatternFormationNeuron::calculateAlphaFromPercentage(real_t alpha) const
{
  return percentage_from_range(alpha, ALPHA_MIN, ALPHA_MAX);
}

real_t PatternFormationNeuron::calculateThetaFromPercentage(real_t theta) const
{
  return percentage_from_range(theta, THETA_MIN, THETA_MAX);
}

// TO PERCENTAGE --------------------------------------------------------------
real_t PatternFormationNeuron::calculateAlphaPercentage(real_t alpha) const
{
  return percentage_of_range(alpha, ALPHA_MIN, ALPHA_MAX);
}

real_t PatternFormationNeuron::calculateThetaPercentage(real_t theta) const
{
  return percentage_of_range(theta, THETA_MIN, THETA_MAX);
}

real_t PatternFormationNeuron::setAlphaPercentage(real_t alpha)
{
  this->alpha = calculateAlphaFromPercentage(alpha);
  return this->alpha;
}

real_t PatternFormationNeuron::setThetaPercentage(real_t theta)
{
  this->theta = calculateThetaFromPercentage(theta);
  return this->theta;
}

