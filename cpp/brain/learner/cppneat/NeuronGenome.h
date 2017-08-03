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

#ifndef REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_NEURONGENOME_H_
#define REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_NEURONGENOME_H_

#include "Genome.h"
#include "CPPNNeuron.h"
#include "CPPNTypes.h"

//class combining meta and normal informations for neurons
namespace cppneat
{
  class NeuronGene
          : public Gene
  {

    public:
    NeuronGene(NeuronPtr neuron,
               int innov_number = 0,
               bool enabled = true,
               std::string parent_name = "",
               int parent_index = -1)
            : Gene(innov_number,
                   enabled,
                   parent_name,
                   parent_index)
            , neuron(neuron)
    {
      this->gene_type = Gene::NEURON_GENE;
    }

    NeuronGene(NeuronGene &copy_of)
            : Gene(copy_of.getInnovNumber(),
                   copy_of.isEnabled(),
                   copy_of.get_parent_name(),
                   copy_of.get_parent_index())
            , neuron(NeuronPtr(new Neuron(*(copy_of.neuron))))
    {
      this->gene_type = Gene::NEURON_GENE;
    }

    public:
    NeuronPtr neuron;
  };
}

#endif // REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_NEURONGENOME_H_
