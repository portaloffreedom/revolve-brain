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

#ifndef CPP_NEAT_ACCNEAT_SRC_NETWORK_CUDA_CUDANETWORKEXECUTOR_H_
#define CPP_NEAT_ACCNEAT_SRC_NETWORK_CUDA_CUDANETWORKEXECUTOR_H_

#pragma once

#include <vector>

#include "network/networkexecutor.h"
#include "cudanetworkbatch.h"

namespace NEAT
{
  ///
  /// CLASS CudaNetworkExecutor
  ///
  template < typename Evaluator >
  class CudaNetworkExecutor
          : public NetworkExecutor< Evaluator >
  {
    std::vector< class CudaNetworkBatch< Evaluator > * > batches;

    public:
    CudaNetworkExecutor()
    {
      int ndevices;
      xcuda(cudaGetDeviceCount(&ndevices));
      errif(ndevices == 0, "No Cuda devices found!");
      batches.resize(ndevices);
      for (int i = 0; i < ndevices; i++)
      {
        batches[i] = new CudaNetworkBatch< Evaluator >(i);
      }
    }

    virtual ~CudaNetworkExecutor()
    {
      for (size_t i = 0; i < batches.size(); i++)
      {
        delete batches[i];
      }
    }

    virtual void configure(
            const typename Evaluator::Config *config,
            size_t len)
    {
      for (size_t i = 0; i < batches.size(); i++)
      {
        batches[i]->configure(config, len);
      }
    }

    virtual void execute(
            class Network **nets_,
            OrganismEvaluation *results,
            size_t nnets)
    {
      CudaNetwork **nets = reinterpret_cast<CudaNetwork **>(nets_);
      size_t nbatches = batches.size();
      uint batch_size = nnets / nbatches;

#pragma omp parallel for
      for (size_t ibatch = 0; ibatch < nbatches; ibatch++)
      {
        size_t inet = ibatch * batch_size;
        size_t n = batch_size;
        if (ibatch == nbatches - 1)
        {
          n += nnets % batch_size;
        }

        batches[ibatch]->activate(nets + inet,
                                  results + inet,
                                  n,
                                  NACTIVATES_PER_INPUT);
      }
    }
  };

  template < typename Evaluator >
  inline NetworkExecutor< Evaluator > *NetworkExecutor< Evaluator >::create()
  {
    return new CudaNetworkExecutor< Evaluator >();
  }
}  // namespace NEAT

#endif
