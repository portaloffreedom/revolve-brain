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

#ifndef REVOLVEBRAIN_BRAIN_CONTROLLER_EXTNN_INPUTNEURON_H_
#define REVOLVEBRAIN_BRAIN_CONTROLLER_EXTNN_INPUTNEURON_H_

#include "ENeuron.h"

namespace revolve
{
  namespace brain
  {

    class InputNeuron
            : public Neuron
    {
      public:
      /// \brief  Constructor for an input neuron
      /// \param id: string to identify the neuron
      /// \param params: parameters that specify the behavior of the neuron
      /// \return pointer to the input neuron
      InputNeuron(const std::string &id,
                  const std::map<std::string, double> &params);

      /// \brief  Method to set the input of the neuron
      /// \param value: new input value
      virtual void SetInput(double value) override;

      /// \brief  Method to calculate the output of the neuron
      /// \param t: current time
      /// \return the output of the neuron at time t
      double CalculateOutput(double t) override;

      /// \brief
      std::map<std::string, double> getNeuronParameters() override;

      /// \brief
      void setNeuronParameters(std::map<std::string, double> params) override;

      /// \brief
      std::string getType() override;

      /// \brief input of the neuron
      protected: double input_;

    };
  }
}

#endif  //  REVOLVEBRAIN_BRAIN_CONTROLLER_EXTNN_INPUTNEURON_H_
