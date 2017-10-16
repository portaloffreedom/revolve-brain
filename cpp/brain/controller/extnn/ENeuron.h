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

#include <cstdlib>
#include <utility>
#include <map>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

/// \brief Types.h
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

    typedef boost::shared_ptr< ExtendedNeuralNetwork > NeuralNetworkPtr;

    typedef boost::shared_ptr< MotorFactory > MotorFactoryPtr;

    typedef boost::shared_ptr< SensorFactory > SensorFactoryPtr;

    typedef boost::shared_ptr< Neuron > NeuronPtr;

    typedef boost::shared_ptr< NeuralConnection > NeuralConnectionPtr;
  }
}

namespace revolve
{
  namespace brain
  {
    class Neuron
    {
      /// \brief Constructor for a neuron.
      /// \param id: string to identify the neuron
      /// \return pointer to the neuron
      public:
      Neuron(const std::string &_id);

      /// \brief
      public:
      virtual ~Neuron()
      {}

      /// \brief Method to calculate the output of the neuron
      /// \param t: current time
      /// \return the output of the neuron at time t
      public:
      virtual double Output(const double _time) = 0;

      /// \brief Return the current output of the neuron
      public:
      double Output() const;

      /// \brief Add an incoming connection to the neuron.
      /// \param socketName: name of the socket the connection "arrives" at
      /// \param connection: name of the connection to be added
      public:
      void AddIncomingConnection(
              const std::string &socketName,
              NeuralConnectionPtr connection);

      /// \brief Deletes all incoming connections
      public:
      void DeleteIncomingConnections();

      /// \brief Method to set the input of the neuron
      /// \param value: the value the input should be set to
      public:
      virtual void SetInput(const double /*value*/)
      {}

      /// \brief Calculate the output and save it for when FlipState is called
      /// \param t: current time
      public:
      void Update(double t);

      /// \brief Set the output to the already calculated new output
      public:
      void FlipState();

      /// \brief Compute the id for the socket of the next imcoming neural
      /// connection
      /// \return id for the socket of the next imcoming neural connection
      public:
      std::string SocketId() const;

      /// \brief Return id of the neuron
      public:
      virtual const std::string &Id() const;

      /// \brief
      public:
      virtual std::vector< std::pair< std::string, NeuralConnectionPtr>>
      IncomingConnections();

      /// \brief
      public:
      virtual std::map< std::string, double > Parameters() = 0;

      /// \brief
      public:
      virtual void
      SetParameters(std::map< std::string, double > _parameters) = 0;

      /// \brief
      public:
      virtual std::string Type() = 0;

      /// \brief Return phase for RythmGeneration neuron (HACK)
      public:
      virtual double Phase();

      /// \brief reset the current activation of the neuron
      public:
      virtual void reset();

      /// \brief vector of the incoming connections and the name of their socket
      protected:
      std::vector< std::pair< std::string, NeuralConnectionPtr>>
              incomingConnections_;

      /// \brief current output
      protected:
      double output_;

      /// \brief calculated output for when FlipState is called
      protected:
      double newOutput_;

      /// \brief id of the neuron
      protected:
      std::string id_;
    };
  }
}

#endif  //  REVOLVEBRAIN_BRAIN_CONTROLLER_EXTNN_NEURON_H_
