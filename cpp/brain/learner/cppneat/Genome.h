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

    Gene(int innov_number = 0,
         bool enabled = true,
         std::string parent_name = "",
         int parent_index = -1)
            : innov_number(innov_number)
              , enabled(enabled)
              , parent_name(parent_name)
              , parent_index(parent_index)
    {}

    virtual inline size_t getInnovNumber()
    {
      return innov_number;
    }

    virtual inline bool isEnabled()
    {
      return enabled;
    }

    virtual inline void setEnabled(bool enabled)
    {
      this->enabled = enabled;
    }

    virtual inline std::string get_parent_name()
    {
      return this->parent_name;
    }

    virtual inline int get_parent_index()
    {
      return this->parent_index;
    }

    GeneType gene_type;

    private:
    unsigned int innov_number;

    private:
    bool enabled;

    private:
    std::string parent_name;

    private:
    int parent_index;
  };
}

#endif  //  REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_GENEOME_H_
