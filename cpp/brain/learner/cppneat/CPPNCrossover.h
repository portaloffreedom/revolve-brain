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
* Author: Rafael Kiesel
*
*/

#ifndef REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_CROSSOVER_H_
#define REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_CROSSOVER_H_

#include "GeneticEncoding.h"

//crossover between genotypes
namespace cppneat
{
  class Crossover
  {
    public:
    static GeneticEncodingPtr
    crossover(GeneticEncodingPtr genotype_more_fit,
              GeneticEncodingPtr genotype_less_fit);

  };
}

#endif  //  REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_CROSSOVER_H_
