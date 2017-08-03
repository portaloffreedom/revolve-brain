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

#ifndef REVOLVEBRAIN_BRAIN_CONTROLLER_EXTNN_NEURALCONNECTION_H_
#define REVOLVEBRAIN_BRAIN_CONTROLLER_EXTNN_NEURALCONNECTION_H_

#include "ENeuron.h"

namespace revolve
{
  namespace brain
  {

    class NeuralConnection
    {
      public:
      /// \brief Consturctor for a neural connetion
      /// \param src: the beginning of the connetion
      /// \param dst: the end of the connection
      /// \param weight: weighting factor of the connection
      /// \return pointer to the neural connection
      NeuralConnection(const NeuronPtr &src,
                       const NeuronPtr &dst,
                       double weight);

      /// \brief Return the weight of the neural connection
      double GetWeight() const;


      /// \param weight: new weight of the neural connection
      void SetWeight(double weigth);

      /// \brief Rreturn the neuron in the beginning of the connection
      NeuronPtr GetInputNeuron() const;

      NeuronPtr GetOutputNeuron() const;

      /// \brief weight of the connection
      protected: double weight_;

      /// \brief source neuron of the connection
      protected: NeuronPtr src_;

      /// \brief destination neuron of the connection
      protected: NeuronPtr dst_;
    };


  }
}

#endif // REVOLVEBRAIN_BRAIN_CONTROLLER_EXTNN_NEURALCONNECTION_H_
