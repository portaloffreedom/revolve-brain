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
* Description: Rafael Kiesel
* Author: TODO <Add proper author>
*
*/

#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "DifferentialCPG.h"

namespace revolve
{
  namespace brain
  {
    DifferentialCPG::DifferentialCPG(
            const std::string &_id,
            const std::map< std::string, double > &_parameters
    )
            : Neuron(_id)
    {
      if (not _parameters.count("rv:bias"))
      {
        std::cerr
                << "A `"
                << "Differential CPG"
                << "` neuron requires `rv:bias` element."
                << std::endl;
        throw std::runtime_error("Robot brain error");
      }
      this->bias_ = _parameters.find("rv:bias")->second;
      lastTime_ = 0;
    }


    double DifferentialCPG::Output(const double _time)
    {
      double deltaT = _time - lastTime_;
      lastTime_ = _time;

      if (deltaT > 0.1)
      {
        deltaT = 0.1;
      }

      double inputValue = 0;

      for (auto it = this->incomingConnections_.begin();
           it not_eq this->incomingConnections_.end(); ++it)
      {
        auto inConnection = it->second;
        inputValue +=
                inConnection->GetInputNeuron()->Output()
                * inConnection->GetWeight();
      }

      double state_deriv = inputValue - this->bias_;
      double result = this->output_ + deltaT * state_deriv;

      double maxOut = 10000.0;

      // limit output:
      // if (result > maxOut) {
      //  result = maxOut;
      // }
      // else if (result < -maxOut) {
      //  result = -maxOut;
      // }

      // saturate output:
      double gain = 2.0 / maxOut;
      result = (2.0 / (1.0 + std::exp(-result * gain)) - 1.0) * maxOut;

      return result;
    }

    std::map< std::string, double > DifferentialCPG::Parameters()
    {
      std::map< std::string, double > parameters;
      parameters["rv:bias"] = bias_;
      return parameters;
    }

    void
    DifferentialCPG::SetParameters(std::map< std::string, double > _parameters)
    {
      if (not _parameters.count("rv:bias"))
      {
        std::cerr
                << "A `"
                << "Differential CPG"
                << "` neuron requires `rv:bias` element."
                << std::endl;
        throw std::runtime_error("Robot brain error");
      }
      this->bias_ = _parameters.find("rv:bias")->second;
    }

    std::string DifferentialCPG::Type()
    {
      return "DifferentialCPG";
    }
  }
}
