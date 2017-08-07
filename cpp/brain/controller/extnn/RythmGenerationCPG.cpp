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
* Author: Milan Jelisavcic
* Date: August 2, 2017.
*
*/

#include <cmath>
#include <iostream>
#include <map>
#include <sstream>

#include "RythmGenerationCPG.h"

namespace revolve
{
  namespace brain
  {
    RythmGenerationCPG::RythmGenerationCPG(
            const std::string &id,
            const std::map< std::string, double > &params)
            : Neuron(id)
            , phi(1)
            , weight(1)
            , amplitude(1)
            , offset(0)
    {
      if (not params.count("rv:bias"))
      {
        std::cerr << "A `RythmGeneration CPG`"
                  << " neuron requires `rv:bias` element." << std::endl;
        throw std::runtime_error("Robot brain error");
      }
      this->bias_ = params.find("rv:bias")->second;
      lastTime_ = 0;
    }


    double RythmGenerationCPG::CalculateOutput(double t)
    {
      double deltaT = t - lastTime_;
      lastTime_ = t;

      if (deltaT > 0.1)
      {
        deltaT = 0.1;
      }

      //      double inputValue = 0;
      static const double PI = std::acos(-1);

      double thisPhi = this->phi;
      double otherPhi = 2 * PI * this->bias_;;

      // create phi(t+1)
      for (auto it = this->incomingConnections_.begin();
           it != this->incomingConnections_.end(); ++it)
      {
        auto inConnection = it->second;
        if ("RythmGeneratorCPG" == inConnection->GetInputNeuron()->getType())
        {
          otherPhi +=
                  std::sin(inConnection->GetInputNeuron()->Phase() - thisPhi)
                  * inConnection->GetWeight();
        }
      }

      thisPhi += (otherPhi * deltaT);
      double result = (this->amplitude * std::cos(thisPhi)) + this->offset;

      // create phi(t+1)
      this->phi = thisPhi;
      return result;
    }

    std::map< std::string, double > RythmGenerationCPG::getNeuronParameters()
    {
      std::map< std::string, double > parameters;
      parameters["rv:bias"] = bias_;
      return parameters;
    }

    void RythmGenerationCPG::setNeuronParameters(
            std::map< std::string, double > params)
    {
      if (not params.count("rv:bias"))
      {
        std::cerr << "A `RythmGeneration CPG`"
                  << " neuron requires `rv:bias` element." << std::endl;
        throw std::runtime_error("Robot brain error");
      }
      this->bias_ = params.find("rv:bias")->second;
    }

    double RythmGenerationCPG::Phase()
    {
      return this->phi;
    }

    std::string RythmGenerationCPG::getType()
    {
      return "RythmGenerationCPG";
    }
  }
}
