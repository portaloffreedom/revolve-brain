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

#ifndef REVOLVEBRAIN_BRAIN_SIMPLESPLITBRAIN_H_
#define REVOLVEBRAIN_BRAIN_SIMPLESPLITBRAIN_H_

#include <iostream>
#include <string>

#include "ConverterSplitBrain.h"
#include "Evaluator.h"

namespace revolve
{
  namespace brain
  {
    template <typename Genotype>
    Genotype convertPolicyToPolicy(Genotype genotype)
    {
      return genotype;
    }

    template <typename Genotype>
    class SimpleSplitBrain
            : public ConverterSplitBrain<Genotype, Genotype>
    {
      public:
      SimpleSplitBrain(const std::string model_name)
              : ConverterSplitBrain<Genotype, Genotype>
                        (&convertPolicyToPolicy,
                         &convertPolicyToPolicy,
                         model_name)
      {}

      virtual ~SimpleSplitBrain()
      {}
    };
  }
}

#endif  //  REVOLVEBRAIN_BRAIN_SIMPLESPLITBRAIN_H_
