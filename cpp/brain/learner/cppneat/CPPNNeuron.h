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

#ifndef REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_NEURON_H_
#define REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_NEURON_H_

#include <map>
#include <string>
#include <vector>

//class containg all information that is evolved for neurons
namespace CPPNEAT
{
  class Neuron
  {
    public:
    enum Layer
    {
      INPUT_LAYER,
      OUTPUT_LAYER,
      HIDDEN_LAYER
    };

    enum Ntype
    {
      SIMPLE = 0,
      SIGMOID = 1,
      DIFFERENTIAL_CPG = 2,
      INPUT = 3,
      BIAS = 4,
      OSCILLATOR = 5,
      INPUT_OSCILLATOR = 6,
      RYTHM_GENERATOR_CPG = 7
    };

    struct ParamSpec
    {
      std::string name;
      double min_value;
      double max_value;
      bool max_inclusive;
      bool min_inclusive;
      double epsilon;
    };

    struct NeuronTypeSpec
    {
      std::vector<ParamSpec> param_specs;
      std::vector<Layer> possible_layers;
    };

    Neuron(Neuron &copy_of);

    Neuron(std::string neuron_id,
           Layer layer,
           Ntype neuron_type,
           std::map<std::string, double> neuron_params);

    void set_neuron_param(double value, ParamSpec param_spec);

    /**
     * tostring     def __str__(self):
     * return "id={0}, layer={1}, type={2}, part={3}".format(self.neuron_id, self.layer, self.neuron_type, self.body_part_id)
     */

    std::string neuron_id;

    Layer layer;

    Ntype neuron_type;

    std::map<std::string, double> neuron_params;
  };
}

#endif  // REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_NEURON_H_
