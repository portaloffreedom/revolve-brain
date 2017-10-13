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
* Description: Class containg meta information about a gene
* Author: Rafael Kiesel
*
*/

#ifndef REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_GENEOME_H_
#define REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_GENEOME_H_

#include <string>

namespace cppneat
{
  class Gene
  {
    public:
    enum GeneType
    {
      NEURON_GENE,
      CONNECTION_GENE
    };

    Gene(const size_t _innovationNumber = 0,
         bool _enabled = true,
         std::string _name = "",
         int _index = -1
    )
            : innovationNumber_(_innovationNumber)
            , isEnabled_(_enabled)
            , parentsName_(_name)
            , parentsIndex_(_index)
    {
    }

    virtual inline size_t InnovationNumber()
    {
      return this->innovationNumber_;
    }

    virtual inline bool IsEnabled()
    {
      return this->isEnabled_;
    }

    virtual inline void SetEnabled(bool enabled)
    {
      this->isEnabled_ = enabled;
    }

    virtual inline std::string ParentsName()
    {
      return this->parentsName_;
    }

    virtual inline int ParentsIndex()
    {
      return this->parentsIndex_;
    }

    GeneType type_;

    private:
    size_t innovationNumber_;

    private:
    bool isEnabled_;

    private:
    std::string parentsName_;

    private:
    int parentsIndex_;
  };
}

#endif  //  REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_GENEOME_H_
