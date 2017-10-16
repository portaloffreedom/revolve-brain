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
* Author: Rafael Kiesel
*
*/

#include <cmath>
#include <iostream>
#include <map>
#include <string>

#include "LeakyIntegrator.h"

namespace revolve
{
  namespace brain
  {
    LeakyIntegrator::LeakyIntegrator(
            const std::string &_id,
            const std::map< std::string, double > &_parameters)
            : Neuron(_id)
    {
      if (not _parameters.count("rv:bias")
          or not _parameters.count("rv:tau"))
      {
        std::cerr << "A `Leaky Integrator`"
                  << " neuron requires `rv:bias` and `rv:tau` elements."
                  << std::endl;
        throw std::runtime_error("Robot brain error");
      }

      this->bias_ = _parameters.find("rv:bias")->second;
      this->tau_ = _parameters.find("rv:tau")->second;

      this->lastTime_ = 0;
      this->stateDeriv_ = 0;
      this->state_ = 0;
    }

    double LeakyIntegrator::Output(const double _time)
    {
      double deltaT = _time - lastTime_;
      lastTime_ = _time;

      if (deltaT > 0.1)
      {
        deltaT = 0.1;
      }

      double inputValue = 0;

      // Calculate the input value
      for (auto it = this->incomingConnections_.begin();
           it != this->incomingConnections_.end(); ++it)
      {
        auto inConnection = it->second;
        inputValue +=
                inConnection->GetInputNeuron()->Output()
                * inConnection->GetWeight();
      }

      stateDeriv_ = (-state_ + inputValue) / tau_;
      state_ = state_ + deltaT * stateDeriv_;

      double result = 1.0 / (1.0 + exp(state_ + bias_));

      return result;
    }

    std::map<std::string, double> LeakyIntegrator::Parameters()
    {
      std::map<std::string, double> ret;
      ret["rv:bias"] = bias_;
      ret["rv:tau"] = tau_;
      return ret;
    }

    void
    LeakyIntegrator::SetParameters(std::map< std::string, double > _parameters)
    {
      if (not _parameters.count("rv:bias") || not _parameters.count("rv:tau"))
      {
        std::cerr
                << "A `"
                << "Leaky Integrator"
                <<
                "` neuron requires `rv:bias` and `rv:tau` elements."
                << std::endl;
        throw std::runtime_error("Robot brain error");
      }

      this->bias_ = _parameters.find("rv:bias")->second;
      this->tau_ = _parameters.find("rv:tau")->second;

      this->stateDeriv_ = 0;
      this->state_ = 0;
    }

    std::string LeakyIntegrator::Type()
    {
      return "LeakyIntegrator";
    }

    void LeakyIntegrator::reset()
    {
      this->stateDeriv_ = 0;
      this->state_ = 0;
      this->output_ = 0;
    }
  }
}
