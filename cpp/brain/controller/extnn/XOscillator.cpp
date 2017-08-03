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

#include "XOscillator.h"
#include <iostream>

namespace revolve {
namespace brain {


XOscillator::XOscillator(const std::string &id,
                         const std::map<std::string, double> &params) :
        Neuron(id)
{
  if (not params.count("rv:tau")) {
    std::cerr << "A `" << "X-Oscillator" <<
              "` neuron requires an `rv:tau` element." << std::endl;
    throw std::runtime_error("Robot brain error");
  }

  this->tau_ = params.find("rv:tau")->second;

  this->lastTime_ = 0;
  this->stateDeriv_ = 0;
}


double
XOscillator::CalculateOutput(double t)
{
  double deltaT = t - lastTime_;
  lastTime_ = t;

  if (deltaT > 0.1) {
    deltaT = 0.1;
  }

  double vInput = 0;  // input from V-neuron of the same oscillator
  double xInput = this->output_; // input from X-neuron of the same oscillator (this neuron)

  for (auto it = this->incomingConnections_.begin(); it != this->incomingConnections_.end(); ++it) {
    auto socketId = it->first;
    auto inConnection = it->second;

    if (socketId == "from_v") {
      vInput += inConnection->GetInputNeuron()->GetOutput() * inConnection->GetWeight();
    }
  }

  stateDeriv_ = vInput / tau_;


  double result = xInput + deltaT * stateDeriv_;
  if (result > 1000.0) {
    result = 1000.0;
  }
  if (result < -1000.0) {
    result = -1000.0;
  }

  return result;
}

std::map<std::string, double>
XOscillator::getNeuronParameters()
{
  std::map<std::string, double> ret;
  ret["rv:tau"] = tau_;
  return ret;
}

void
XOscillator::setNeuronParameters(std::map<std::string, double> params)
{
  if (not params.count("rv:tau")) {
    std::cerr << "A `" << "X-Oscillator" <<
              "` neuron requires an `rv:tau` element." << std::endl;
    throw std::runtime_error("Robot brain error");
  }

  this->tau_ = params.find("rv:tau")->second;

  this->stateDeriv_ = 0;
}


std::string
XOscillator::getType()
{
  return "XOscillator";
}


}
}
