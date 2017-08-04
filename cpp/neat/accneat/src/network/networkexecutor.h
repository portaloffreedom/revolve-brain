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

#ifndef CPP_NEAT_ACCNEAT_SRC_NETWORK_NETWORKEXECUTOR_H_
#define CPP_NEAT_ACCNEAT_SRC_NETWORK_NETWORKEXECUTOR_H_

#pragma once

namespace NEAT
{
#define NACTIVATES_PER_INPUT 10

  //---
  //--- CLASS NetworkExecutor<>
  //---
  template <typename Evaluator>
  class NetworkExecutor
  {
    public:
    /// \brief
    static NetworkExecutor *create();

    /// \brief
    virtual ~NetworkExecutor()
    {}

    /// \brief
    virtual void configure(const typename Evaluator::Config *config,
                           size_t len) = 0;

    /// \brief
    virtual void execute(class Network **nets_,
                         struct OrganismEvaluation *results,
                         size_t nnets) = 0;
  };
}

#ifdef ENABLE_CUDA
#include "cudanetworkexecutor.h"
#else
#include "network/cpu/cpunetworkexecutor.h"
#endif

#endif
