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

#include "InputDependentOscillatorNeuron.h"

namespace revolve
{
  namespace brain
  {
    InputDependentOscillatorNeuron::InputDependentOscillatorNeuron(
            const std::string &_id,
            const std::map< std::string, double > &_parameters
    )
            : Neuron(_id)
    {
      if (not _parameters.count("rv:period")
          or not _parameters.count("rv:phase_offset")
          or not _parameters.count("rv:amplitude"))
      {
        std::cerr << "An `Oscillator` neuron requires"
                  << " `rv:period`, `rv:phase_offset` and "
                     "`rv:amplitude` elements."
                  << std::endl;
        throw std::runtime_error("Robot brain error");
      }

      this->period_ = _parameters.find("rv:period")->second;
      this->phaseOffset_ = _parameters.find("rv:phase_offset")->second;
      this->gain_ = _parameters.find("rv:amplitude")->second;
    }


    double InputDependentOscillatorNeuron::Output(const double /*t*/)
    {
      double inputValue = 0;

      for (auto it = this->incomingConnections_.begin();
           it not_eq this->incomingConnections_.end(); ++it)
      {
        auto inConnection = it->second;
        inputValue += inConnection->GetInputNeuron()->Output()
                      * inConnection->GetWeight();
      }
      return 0.5
             * (1.0 + this->gain_
                      * sin(2.0 * M_PI
                            / (this->period_)
                            * (inputValue - this->period_
                                            * this->phaseOffset_)));
    }

    std::map<std::string, double> InputDependentOscillatorNeuron::Parameters()
    {
      std::map<std::string, double> ret;
      ret["rv:period"] = period_;
      ret["rv:phase_offset"] = phaseOffset_;
      ret["rv:amplitude"] = gain_;
      return ret;
    }

    void InputDependentOscillatorNeuron::SetParameters(
            std::map< std::string, double > _parameters)
    {
      if (not _parameters.count("rv:period")
          or not _parameters.count("rv:phase_offset")
          or not _parameters.count("rv:amplitude"))
      {
        std::cerr << "An `Oscillator` neuron requires "
                << "`rv:period`, `rv:phase_offset` and `rv:amplitude` elements"
                << std::endl;
        throw std::runtime_error("Robot brain error");
      }

      this->period_ = _parameters.find("rv:period")->second;
      this->phaseOffset_ = _parameters.find("rv:phase_offset")->second;
      this->gain_ = _parameters.find("rv:amplitude")->second;
    }

    std::string InputDependentOscillatorNeuron::Type()
    {
      return "InputDependentOscillator";
    }
  }
}
