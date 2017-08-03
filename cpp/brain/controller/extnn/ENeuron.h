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

#ifndef REVOLVEBRAIN_BRAIN_CONTROLLER_EXTNN_NEURON_H_
#define REVOLVEBRAIN_BRAIN_CONTROLLER_EXTNN_NEURON_H_

//#include "Types.h"
#include <cstdlib>
#include <utility>
#include <map>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

//Types.h
namespace revolve
{
  namespace brain
  {
    class Motor;

    class VirtualSensor;

    class MotorFactory;

    class SensorFactory;

    class ExtendedNeuralNetwork;

    class Neuron;

    class NeuralConnection;

    typedef boost::shared_ptr<ExtendedNeuralNetwork> NeuralNetworkPtr;

    typedef boost::shared_ptr<MotorFactory> MotorFactoryPtr;

    typedef boost::shared_ptr<SensorFactory> SensorFactoryPtr;

    typedef boost::shared_ptr<Neuron> NeuronPtr;

    typedef boost::shared_ptr<NeuralConnection> NeuralConnectionPtr;
  }
}


namespace revolve
{
  namespace brain
  {

    class Neuron
    {
      public:
      /// \brief Constructor for a neuron.
      /// \param id: string to identify the neuron
      /// \return pointer to the neuron
      Neuron(const std::string &id);

      virtual ~Neuron()
      {};

      /// \brief Method to calculate the output of the neuron
      /// \param t: current time
      /// \return the output of the neuron at time t
      virtual double CalculateOutput(double t) = 0;

      /// \brief Add an incoming connection to the neuron.
      /// \param socketName: name of the socket the connection "arrives" at
      /// \param connection: name of the connection to be added
      void AddIncomingConnection(const std::string &socketName,
                                 NeuralConnectionPtr connection);

      /// \brief Deletes all incoming connections
      void DeleteIncomingConections();

      /// \brief Return the current output of the neuron
      double GetOutput() const;

      /// \brief Method to set the input of the neuron
      /// \param value: the value the input should be set to
      virtual void SetInput(double /*value*/)
      {};

      /// \brief Calculate the output and save it for when FlipState is called
      /// \param t: current time
      void Update(double t);

      /// \brief Set the output to the already calculated new output
      void FlipState();

      /// \brief Compute the id for the socket of the next imcoming neural
      /// connection
      /// \return id for the socket of the next imcoming neural connection
      std::string GetSocketId() const;

      /// \brief Return id of the neuron
      virtual const std::string &Id() const;

      virtual std::vector<std::pair<std::string, NeuralConnectionPtr>>
      getIncomingConnections();

      virtual std::map<std::string, double> getNeuronParameters() = 0;

      virtual void
      setNeuronParameters(std::map<std::string, double> params) = 0;

      virtual std::string getType() = 0;

      /// \brief reset the current activation of the neuron
      virtual void reset();

      /// \brief vector of the incoming connections and the name of their socket
      protected: std::vector<std::pair<std::string, NeuralConnectionPtr>>
              incomingConnections_;

      /// \brief current output
      protected: double output_;

      /// \brief calculated output for when FlipState is called
      protected: double newOutput_;

      /// \brief id of the neuron
      protected: std::string id_;

    };

  }
}
#endif // REVOLVEBRAIN_BRAIN_CONTROLLER_EXTNN_NEURON_H_
