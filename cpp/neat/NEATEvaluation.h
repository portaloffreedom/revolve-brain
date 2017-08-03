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

#ifndef REVOLVE_NEAT_NEATEVALUATION_H_
#define REVOLVE_NEAT_NEATEVALUATION_H_

#include <functional>
#include <iostream>

#include "organism.h"

class NeatEvaluation
{
public:
    NeatEvaluation(NEAT::Organism *organism);

    virtual ~NeatEvaluation()
    {};

    std::function<void(float)>
    add_finished_callback(std::function<void(float)> new_callback)
    {
      std::function<void(float)> old_callback = finished_callback;
      finished_callback = new_callback;
      return old_callback;
    }

    const NEAT::Organism *
    getOrganism() const
    {
      return organism;
    }

    /**
     * Evaluation is finished and fitness is passed to the evaluation
     */
    virtual void
    finish(float fitness);

private:
    std::function<void(float)> finished_callback;
    float fitness;
    NEAT::Organism *organism;
};

#endif // REVOLVE_NEAT_NEATEVALUATION_H_
