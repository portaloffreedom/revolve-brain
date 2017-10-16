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

#include "SigmoidNeuron.h"

namespace revolve
{
  namespace brain
  {
    SigmoidNeuron::SigmoidNeuron(
            const std::string &_id,
            const std::map< std::string, double > &_parameters)
            : Neuron(_id)
    {
      if (not _parameters.count("rv:bias") or not _parameters.count("rv:gain"))
      {
        std::cerr << "A `" << "Sigmoid"
                << "` neuron requires `rv:bias` and `rv:gain` elements."
                << std::endl;
        throw std::runtime_error("Robot brain error");
      }
      this->bias_ = _parameters.find("rv:bias")->second;
      this->gain_ = _parameters.find("rv:gain")->second;
    }


    double SigmoidNeuron::Output(const double /*t*/)
    {
      double inputValue = 0;

      for (const auto &connection : this->incomingConnections_)
      {
        auto inConnection = connection.second;
        inputValue += inConnection->GetInputNeuron()->Output()
                      * inConnection->GetWeight();
      }

      return 1.0 / (1.0 + exp(-this->gain_ * (inputValue - this->bias_)));
    }

    std::map<std::string, double> SigmoidNeuron::Parameters()
    {
      std::map<std::string, double> ret;
      ret["rv:bias"] = bias_;
      ret["rv:gain"] = gain_;
      return ret;
    }

    void
    SigmoidNeuron::SetParameters(std::map< std::string, double > _parameters)
    {
      if (not _parameters.count("rv:bias") or not _parameters.count("rv:gain"))
      {
        std::cerr << "A `" << "Sigmoid"
                  << "` neuron requires `rv:bias` and `rv:gain` elements."
                  << std::endl;
        throw std::runtime_error("Robot brain error");
      }
      this->bias_ = _parameters.find("rv:bias")->second;
      this->gain_ = _parameters.find("rv:gain")->second;
    }

    std::string SigmoidNeuron::Type()
    {
      return "Sigmoid";
    }
  }
}
