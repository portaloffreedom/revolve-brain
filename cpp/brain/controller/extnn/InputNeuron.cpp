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

#include <map>
#include <string>

#include "InputNeuron.h"

namespace revolve
{
  namespace brain
  {
    InputNeuron::InputNeuron(
            const std::string &_id,
            const std::map< std::string, double > &/*_parameters*/
    )
            : Neuron(_id)
    {
      input_ = 0;
    }

    double InputNeuron::Output(const double /*t*/)
    {
      return input_;
    }

    void InputNeuron::SetInput(const double _value)
    {
      input_ = _value;
    }

    std::map< std::string, double > InputNeuron::Parameters()
    {
      std::map< std::string, double > ret;
      return ret;
    }

    void
    InputNeuron::SetParameters(std::map< std::string, double > /*_parameters*/)
    {
    }

    std::string InputNeuron::Type()
    {
      return "Input";
    }
  }
}
