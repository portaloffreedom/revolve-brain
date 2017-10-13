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
* Description: Class combining meta and normal informations for neurons
* Author: TODO <Add proper author>
*
*/

#ifndef REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_NEURONGENOME_H_
#define REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_NEURONGENOME_H_

#include <string>

#include "Genome.h"
#include "CPPNNeuron.h"
#include "CPPNTypes.h"

namespace cppneat
{
  class NeuronGene
          : public Gene
  {
    public:
    NeuronGene(NeuronPtr _neuron,
               size_t _innovationNumber = 0,
               bool _enabled = true,
               std::string _parentsName = "",
               int _parentsIndex = -1
    )
            : Gene(_innovationNumber,
                   _enabled,
                   _parentsName,
                   _parentsIndex)
            , neuron_(_neuron)
    {
      this->type_ = Gene::NEURON_GENE;
    }

    NeuronGene(NeuronGene &_copy)
            : Gene(_copy.InnovationNumber(),
                   _copy.IsEnabled(),
                   _copy.ParentsName(),
                   _copy.ParentsIndex())
            , neuron_(NeuronPtr(new Neuron(*(_copy.neuron_))))
    {
      this->type_ = Gene::NEURON_GENE;
    }

    public: NeuronPtr neuron_;
  };
}

#endif  //  REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_NEURONGENOME_H_
