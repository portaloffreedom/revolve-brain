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
* Description: Specifies a utility `Learner` base class, which is supposed to
* change the genome of the robot.
* Author: Rafael Kiesel
*
*/

#ifndef REVOLVEBRAIN_BRAIN_LEARNER_BRAINLEARNER_H_
#define REVOLVEBRAIN_BRAIN_LEARNER_BRAINLEARNER_H_

#include <string>

namespace revolve
{
  namespace brain
  {
    template <typename Genotype>
    class Learner
    {
      public:

      virtual ~Learner()
      {}

      /// \brief Method to report the fitness of a robot
      /// \param[in] id: identifier of a robot (in case there are multiple ones)
      /// \param[in] genome: genome that was tested on the robot
      /// \param[in] fitness: value of the fitness evaluation
      virtual void reportFitness(const std::string &_id,
                                 Genotype _genotype,
                                 const double _fitness) = 0;


      /// \brief Getter for a new robot genotype
      /// \note reportFitness should be called first so the learner can make
      /// a more informed decision
      /// \param[in] id: identifier of a robot (in case there are multiple ones)
      /// \return new genome
      virtual Genotype currentGenotype() = 0;
    };
  }
}

#endif  //  REVOLVEBRAIN_BRAIN_LEARNER_BRAINLEARNER_H_
