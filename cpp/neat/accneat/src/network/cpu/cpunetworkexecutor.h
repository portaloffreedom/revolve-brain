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

#ifndef CPP_NEAT_ACCNEAT_SRC_NETWORK_CPU_CPUNETWORKEXECUTOR_H_
#define CPP_NEAT_ACCNEAT_SRC_NETWORK_CPU_CPUNETWORKEXECUTOR_H_

#pragma once

#include <cstdlib>
#include <cstring>

#include "cpunetwork.h"
#include "network/networkexecutor.h"

namespace NEAT
{
// Don't need any special qualifiers for CPU
#define __net_eval_decl
  ///
  /// CLASS CpuNetworkExecutor
  ///
  template <typename Evaluator>
  class CpuNetworkExecutor
          : public NetworkExecutor<Evaluator>
  {
    public:
    const typename Evaluator::Config *config;

    CpuNetworkExecutor()
    {
      config = NULL;
    }

    virtual ~CpuNetworkExecutor()
    {
      delete config;
    }

    virtual void configure(const typename Evaluator::Config *config_,
                           size_t len)
    {
      void *buf = std::malloc(len);
      std::memcpy(buf, config_, len);
      config = (const typename Evaluator::Config *)buf;
    }

    virtual void execute(class Network **nets_,
                         OrganismEvaluation *results,
                         size_t nnets)
    {
      CpuNetwork **nets = (CpuNetwork **)nets_;
      node_size_t nsensors = nets[0]->get_dims().nnodes.sensor;

#pragma omp parallel for
      for (size_t inet = 0; inet < nnets; inet++)
      {
        CpuNetwork *net = nets[inet];
        Evaluator eval(config);

        while (eval.next_step())
        {
          if (eval.clear_noninput())
          {
            net->clear_noninput();
          }
          for (node_size_t isensor = 0; isensor < nsensors; isensor++)
          {
            net->load_sensor(isensor, eval.get_sensor(isensor));
          }
          net->activate(NACTIVATES_PER_INPUT);
          eval.evaluate(net->get_outputs());
        }

        results[inet] = eval.result();
      }
    }
  };

  ///
  /// FUNC NetworkExecutor<Evaluator>::create()
  ///
  template <typename Evaluator>
  inline NetworkExecutor<Evaluator> *NetworkExecutor<Evaluator>::create()
  {
    return new CpuNetworkExecutor<Evaluator>();
  }
}

#endif
