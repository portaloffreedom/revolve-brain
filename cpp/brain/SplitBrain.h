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

/*
 * Specifies a utility `SplitBrain` base class. Here the learner and controller are separeted explicitly.
 */

#ifndef REVOLVEBRAIN_BRAIN_SPLITBRAIN_H_
#define REVOLVEBRAIN_BRAIN_SPLITBRAIN_H_

#include "Brain.h"

#include "brain/controller/Controller.h"
#include "brain/learner/Learner.h"

namespace revolve {
namespace brain {

template <typename Phenotype, typename Genotype>
class SplitBrain
        : public Brain
{
public:

    virtual ~SplitBrain() {};

protected:

    /// control responsible for the movement of the robot
    boost::shared_ptr<Controller<Phenotype>> controller_;

    /// learner used to get new genomes
    boost::shared_ptr<Learner<Genotype>> learner_;
};

}
}

#endif  //  REVOLVEBRAIN_BRAIN_SPLITBRAIN_H_
