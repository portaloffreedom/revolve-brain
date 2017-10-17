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
* Description: Class containg all information that is evolved for neurons
* Author: Rafael Kiesel
*
*/

#ifndef REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_NEURON_H_
#define REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_NEURON_H_

#include <map>
#include <string>
#include <vector>

namespace cppneat
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
      double min;
      double max;
      bool maxInclusive;
      bool minInclusive;
      double epsilon;
    };

    struct NeuronTypeSpec
    {
      std::vector< ParamSpec > parameters;
      std::vector< Layer > possibleLayers;
    };

    Neuron(Neuron &_copy);

    Neuron(
            const std::string &_neuronId,
            Layer _layer,
            Ntype _neuronType,
            std::map< std::string, double > &_parameters);

    void SetNeuronParameters(
            const double _value,
            ParamSpec _parameters);

    std::string neuronId_;

    Layer layer_;

    Ntype neuronType_;

    std::map< std::string, double > parameters_;
  };
}

#endif  // REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_NEURON_H_
