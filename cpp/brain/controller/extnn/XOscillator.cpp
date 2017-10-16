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

#include <map>
#include <iostream>
#include <string>

#include "XOscillator.h"

namespace revolve
{
  namespace brain
  {
    XOscillator::XOscillator(
            const std::string &_id,
            const std::map< std::string, double > &_parameters
    )
            : Neuron(_id)
    {
      if (not _parameters.count("rv:tau"))
      {
        std::cerr << "A `" << "X-Oscillator" <<
                  "` neuron requires an `rv:tau` element." << std::endl;
        throw std::runtime_error("Robot brain error");
      }

      this->tau_ = _parameters.find("rv:tau")->second;
      this->lastTime_ = 0;
      this->stateDeriv_ = 0;
    }

    double XOscillator::Output(const double _time)
    {
      double deltaT = _time - lastTime_;
      lastTime_ = _time;

      if (deltaT > 0.1)
      {
        deltaT = 0.1;
      }

      // input from V-neuron of the same oscillator
      double vInput = 0;
      // input from X-neuron of the same oscillator (this neuron)
      double xInput = this->output_;

      for (const auto &connection : this->incomingConnections_)
      {
        auto socketId = connection.first;
        auto inConnection = connection.second;

        if (socketId == "from_v")
        {
          vInput += inConnection->GetInputNeuron()->Output()
                    * inConnection->GetWeight();
        }
      }

      this->stateDeriv_ = vInput / tau_;

      double result = xInput + deltaT * this->stateDeriv_;
      if (result > 1000.0)
      {
        result = 1000.0;
      }
      if (result < -1000.0)
      {
        result = -1000.0;
      }

      return result;
    }

    std::map<std::string, double> XOscillator::Parameters()
    {
      std::map<std::string, double> ret;
      ret["rv:tau"] = tau_;
      return ret;
    }

    void
    XOscillator::SetParameters(std::map< std::string, double > _parameters)
    {
      if (not _parameters.count("rv:tau"))
      {
        std::cerr << "A `" << "X-Oscillator" <<
                  "` neuron requires an `rv:tau` element." << std::endl;
        throw std::runtime_error("Robot brain error");
      }

      this->tau_ = _parameters.find("rv:tau")->second;

      this->stateDeriv_ = 0;
    }

    std::string XOscillator::Type()
    {
      return "XOscillator";
    }
  }
}
