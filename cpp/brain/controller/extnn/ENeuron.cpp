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

#include <iostream>

#include "ENeuron.h"

namespace revolve
{
  namespace brain
  {
    Neuron::Neuron(const std::string &id)
    {
      this->output_ = 0;
      this->newOutput_ = 0;
      this->id_ = id;
    }

    void Neuron::AddIncomingConnection(const std::string &socketName,
                                       NeuralConnectionPtr connection)
    {
      this->incomingConnections_.push_back(
              std::pair<std::string, NeuralConnectionPtr>(socketName,
                                                          connection)
      );
    }

    void Neuron::DeleteIncomingConections()
    {
      incomingConnections_.clear();
    }

    void Neuron::Update(double t)
    {
      this->newOutput_ = this->CalculateOutput(t);
    }

    void Neuron::FlipState()
    {
      this->output_ = this->newOutput_;
    }

    double Neuron::GetOutput() const
    {
      return this->output_;
    }

    std::string Neuron::GetSocketId() const
    {
      return std::to_string(this->incomingConnections_.size());
    }

    const std::string &Neuron::Id() const
    {
      return this->id_;
    }

    std::vector<std::pair<std::string, NeuralConnectionPtr> >
    Neuron::getIncomingConnections()
    {
      return this->incomingConnections_;
    }

    double Neuron::Phase()
    {
      return 0.0;
    }

    void Neuron::reset()
    {
      this->output_ = 0;
    }
  }
}
