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
* Description: Class for connection genes
* Author: Rafael Kiesel
*
*/

#ifndef REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_CONNECTIONGENOME_H_
#define REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_CONNECTIONGENOME_H_

#include <string>

#include "Genome.h"

namespace cppneat
{
  class ConnectionGene
          : public Gene
  {
    public:
    /// \brief
    ConnectionGene(int mark_to,
                   int mark_from,
                   double weight,
                   int innov_number = 0,
                   bool enabled = true,
                   std::string parent_name = "",
                   int parent_index = -1,
                   std::string socket = "")
            : Gene(innov_number, enabled, parent_name, parent_index)
              , mark_to(mark_to)
              , mark_from(mark_from)
              , weight(weight)
              , socket(socket)
    {
      this->gene_type = Gene::CONNECTION_GENE;
    }

    /// \brief
    ConnectionGene(ConnectionGene &copy_of)
            : Gene(copy_of.getInnovNumber(),
                   copy_of.isEnabled(),
                   copy_of.get_parent_name(),
                   copy_of.get_parent_index())
              , mark_to(copy_of.mark_to)
              , mark_from(copy_of.mark_from)
              , weight(copy_of.weight)
              , socket(copy_of.socket)
    {
      this->gene_type = Gene::CONNECTION_GENE;
    }

    public:
    /// \brief
    int mark_to;

    /// \brief
    int mark_from;

    /// \brief
    double weight;

    /// \brief
    std::string socket;
  };
}

#endif  //  REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_CONNECTIONGENOME_H_
