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

#include <map>
#include <string>

#include "CPPNNeuron.h"

namespace cppneat
{
  Neuron::Neuron(Neuron &_copy)
          : neuronId_(_copy.neuronId_)
            , layer_(_copy.layer_)
            , neuronType_(_copy.neuronType_)
            , parameters_(_copy.parameters_)
  {
  }

  Neuron::Neuron(
          const std::string &_neuronId,
          Layer _layer,
          Ntype _neuronType,
          std::map< std::string, double > _parameters)
          : neuronId_(_neuronId)
            , layer_(_layer)
            , neuronType_(_neuronType)
            , parameters_(_parameters)
  {
  }

  void Neuron::SetNeuronParameters(
          const double _value,
          ParamSpec _parameters)
  {
    parameters_[_parameters.name] = _value;
  }
}
