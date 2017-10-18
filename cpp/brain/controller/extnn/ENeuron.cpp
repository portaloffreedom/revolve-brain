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
#include <string>
#include <utility>
#include <vector>

#include "ENeuron.h"

namespace revolve
{
  namespace brain
  {
    Neuron::Neuron(const std::string &_id)
            : output_(0)
            , newOutput_(0)
            , id_(_id)
    {
    }

    void Neuron::AddIncomingConnection(
            const std::string &socketName,
            NeuralConnectionPtr connection)
    {
      this->incomingConnections_.push_back({socketName, connection});
    }

    void Neuron::DeleteIncomingConnections()
    {
      incomingConnections_.clear();
    }

    void Neuron::Update(double t)
    {
      this->newOutput_ = this->Output(t);
    }

    void Neuron::FlipState()
    {
      this->output_ = this->newOutput_;
    }

    double Neuron::Output() const
    {
      return this->output_;
    }

    std::string Neuron::SocketId() const
    {
      return std::to_string(this->incomingConnections_.size());
    }

    const std::string &Neuron::Id() const
    {
      return this->id_;
    }

    std::vector< std::pair< std::string, NeuralConnectionPtr > >
    Neuron::IncomingConnections()
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
