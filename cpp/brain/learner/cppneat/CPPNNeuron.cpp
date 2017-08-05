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
  Neuron::Neuron(Neuron &copy_of)
          : neuron_id(copy_of.neuron_id)
            , layer(copy_of.layer)
            , neuron_type(copy_of.neuron_type)
            , neuron_params(copy_of.neuron_params)
  {
  }

  Neuron::Neuron(std::string neuron_id,
                 Layer layer,
                 Ntype neuron_type,
                 std::map<std::string, double> neuron_params)
          : neuron_id(neuron_id)
            , layer(layer)
            , neuron_type(neuron_type)
            , neuron_params(neuron_params)
  {
  }

  void Neuron::set_neuron_param(double value,
                                ParamSpec param_spec)
  {
    //  if(value > param_spec.max_value) {
    //    if(param_spec.max_inclusive) {
    //      value = param_spec.max_value;
    //    }
    //    else {
    //      value = param_spec.max_value - param_spec.epsilon;
    //    }
    //  }
    //  if(value < param_spec.min_value) {
    //    if(param_spec.min_inclusive) {
    //      value = param_spec.min_value;
    //    }
    //    else {
    //      value = param_spec.min_value + param_spec.epsilon;
    //    }
    //  }
    neuron_params[param_spec.name] = value;
  }
}
