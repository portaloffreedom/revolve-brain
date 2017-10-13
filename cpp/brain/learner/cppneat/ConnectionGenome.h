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
    ConnectionGene(
            size_t _to,
            size_t _from,
            double _weight,
            int _innovationNumber = 0,
            bool _enabled = true,
            std::string _parentName = "",
            int _parentIndex = -1,
            std::string _socket = ""
    )
            : Gene(_innovationNumber, _enabled, _parentName, _parentIndex)
            , to_(_to)
            , from_(_from)
            , weight_(_weight)
            , socket_(_socket)
    {
      this->type_ = Gene::CONNECTION_GENE;
    }

    /// \brief
    ConnectionGene(ConnectionGene &_copy)
            : Gene(_copy.InnovationNumber(),
                   _copy.IsEnabled(),
                   _copy.ParentsName(),
                   _copy.ParentsIndex())
            , to_(_copy.to_)
            , from_(_copy.from_)
            , weight_(_copy.weight_)
            , socket_(_copy.socket_)
    {
      this->type_ = Gene::CONNECTION_GENE;
    }

    public:
    /// \brief
    size_t to_;

    /// \brief
    size_t from_;

    /// \brief
    double weight_;

    /// \brief
    std::string socket_;
  };
}

#endif  //  REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_CONNECTIONGENOME_H_
