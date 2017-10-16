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

#include <cmath>
#include <iostream>
#include <map>
#include <string>

#include "VOscillator.h"

namespace revolve
{
  namespace brain
  {
    VOscillator::VOscillator(
            const std::string &_id,
            const std::map< std::string, double > &_parameters
    )
            : Neuron(_id)
    {
      if (not _parameters.count("rv:alpha")
          or not _parameters.count("rv:tau")
          or not _parameters.count("rv:energy"))
      {
        std::cerr << "A ` V-Oscillator` neuron requires"
                  << " `rv:alpha`, `rv:tau` and `rv:energy` elements."
                  << std::endl;
        throw std::runtime_error("Robot brain error");
      }

      this->alpha_ = _parameters.find("rv:alpha")->second;
      this->tau_ = _parameters.find("rv:tau")->second;
      this->energy_ = _parameters.find("rv:energy")->second;

      this->lastTime_ = 0;
      this->stateDeriv_ = 0;
      this->output_ = sqrt(this->energy_);
    }

    double VOscillator::Output(const double _time)
    {
      double deltaT = _time - lastTime_;
      lastTime_ = _time;

      if (deltaT > 0.1)
      {
        deltaT = 0.1;
      }

      // input from X-neuron of the same oscillator
      double xInput = 0;
      // input from V-neuron of the same oscillator (this neuron)
      double vInput = this->output_;
      // input from V-neuron of another oscillator
      double vExternal = 0;
      // input from X-neuron of another oscillator
      double xExternal = 0;
      // all other inputs
      double otherInputs = 0;

      for (const auto &connection : this->incomingConnections_)
      {
        auto socketId = connection.first;
        auto inConnection = connection.second;

        if (socketId == "from_x")
        {
          xInput += inConnection->GetInputNeuron()->Output()
                    * inConnection->GetWeight();
        }
        else if (socketId == "from_x_ext")
        {
          xExternal += inConnection->GetInputNeuron()->Output()
                       * inConnection->GetWeight();
        }
        else if (socketId == "from_v_ext")
        {
          vExternal += inConnection->GetInputNeuron()->Output()
                       * inConnection->GetWeight();
        }
        else
        {
          otherInputs += inConnection->GetInputNeuron()->Output()
                         * inConnection->GetWeight();
        }
      }

      stateDeriv_ =
              (-(alpha_ / energy_)
               * vInput * (xInput * xInput + vInput * vInput)
               + alpha_ * vInput - xInput + xExternal + vExternal + otherInputs)
              / tau_;

      double result = vInput + deltaT * stateDeriv_;

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

    std::map<std::string, double> VOscillator::Parameters()
    {
      std::map<std::string, double> parameters;
      parameters["rv:alpha"] = alpha_;
      parameters["rv:tau"] = tau_;
      parameters["rv:energy"] = energy_;
      return parameters;
    }

    void VOscillator::SetParameters(std::map< std::string, double > _parameters)
    {
      if (not _parameters.count("rv:alpha")
          or not _parameters.count("rv:tau")
          or not _parameters.count("rv:energy"))
      {
        std::cerr << "A `V-Oscillator` neuron requires"
                  << " `rv:alpha`, `rv:tau` and `rv:energy` elements."
                  << std::endl;
        throw std::runtime_error("Robot brain error");
      }

      this->alpha_ = _parameters.find("rv:alpha")->second;
      this->tau_ = _parameters.find("rv:tau")->second;
      this->energy_ = _parameters.find("rv:energy")->second;

      this->stateDeriv_ = 0;
      this->output_ = sqrt(this->energy_);
    }

    std::string VOscillator::Type()
    {
      return "VOscillator";
    }

    void VOscillator::reset()
    {
      this->stateDeriv_ = 0;
      this->output_ = sqrt(this->energy_);
    }
  }
}
