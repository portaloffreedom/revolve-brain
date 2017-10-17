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
* Author: Rafael Kiesel
*
*/

#ifndef REVOLVEBRAIN_BRAIN_CONTROLLER_LAYEREDEXTENDEDANN_H_
#define REVOLVEBRAIN_BRAIN_CONTROLLER_LAYEREDEXTENDEDANN_H_

#include <map>
#include <vector>
#include <string>

#include "Controller.h"

#include "brain/Evaluator.h"
#include "brain/controller/extnn/ENeuron.h"
#include "brain/controller/extnn/NeuralConnection.h"
#include "brain/controller/extnn/LinearNeuron.h"
#include "brain/controller/extnn/SigmoidNeuron.h"
#include "brain/controller/extnn/OscillatorNeuron.h"
#include "brain/controller/extnn/VOscillator.h"
#include "brain/controller/extnn/XOscillator.h"
#include "brain/controller/extnn/LeakyIntegrator.h"
#include "brain/controller/extnn/BiasNeuron.h"
#include "brain/controller/extnn/DifferentialCPG.h"
#include "brain/controller/extnn/RythmGenerationCPG.h"
#include "brain/controller/extnn/InputNeuron.h"
#include "brain/controller/extnn/InputDependentOscillatorNeuron.h"

namespace revolve
{
  namespace brain
  {
    struct LayeredExtNNConfig
    {
      // vector containing all neurons
      std::vector< std::vector< NeuronPtr>> layers_;
      // positions for indexing into the outputs_ buffer for each output neuron
      std::map< NeuronPtr, int > outputPositionMap_;
      // positions for indexing into the inputs_ buffer for each input neuron
      std::map< NeuronPtr, int > inputPositionMap_;
      // Map neuron id strings to Neuron objects
      std::map< std::string, NeuronPtr > idToNeuron_;
      // vector of all the neural connections
      std::vector< NeuralConnectionPtr > connections_;
    };

    /// \brief extended neural network controller usable with standard neat or
    /// hyper neat (use different conversion methods)
    class LayeredExtNNController
            : public Controller< boost::shared_ptr< LayeredExtNNConfig>>
    {
      public:
      /// \brief Constructor for a neural network including neurons that are of
      /// a different type than the usual ones.
      /// \param modelName: name of the model
      /// \param Config: configuration file
      /// \param evaluator: pointer to the evaluator that is used
      /// \param actuators: vector list of robot's actuators
      /// \param sensors: vector list of robot's sensors
      /// \return pointer to the neural network
      LayeredExtNNController(
              std::string modelName,
              boost::shared_ptr< LayeredExtNNConfig > Config,
              const std::vector< ActuatorPtr > &actuators,
              const std::vector< SensorPtr > &sensors);

      virtual ~LayeredExtNNController();

      /// \brief Method for updating sensors readings, actuators positions
      /// \param actuators: vector list of robot's actuators
      /// \param sensors: vector list of robot's sensors
      /// \param t: current time
      /// \param step:
      virtual void update(
              const std::vector< ActuatorPtr > &actuators,
              const std::vector< SensorPtr > &sensors,
              double t,
              double step);

      /// \brief Gets the weight of all the connections
      /// \return weights of all neural connections
      virtual boost::shared_ptr< LayeredExtNNConfig > getGenotype();

      /// \brief Changes the weights of the neural connections
      /// \param weights: new weights to be assigned
      virtual void setGenotype(boost::shared_ptr< LayeredExtNNConfig > config);

      /// \brief
      void writeNetwork(std::ofstream &write_to);

      protected:

      /// \brief name of the robot
      std::string modelName_;

      /// \brief buffer of input values from the sensors
      double *inputs_;

      /// \brief buffer of output values for the actuators
      double *outputs_;

      /// \brief vector containing all neurons
      std::vector< std::vector< NeuronPtr>> layers_;

      /// \brief positions for indexing into the outputs_ buffer for each
      /// output neuron
      std::map< NeuronPtr, int > outputPositionMap_;

      /// \brief positions for indexing into the inputs_ buffer for each
      /// input neuron
      std::map< NeuronPtr, int > inputPositionMap_;

      /// \brief Map neuron id strings to Neuron objects
      std::map< std::string, NeuronPtr > idToNeuron_;

      /// \brief vector of all the neural connections
      std::vector< NeuralConnectionPtr > connections_;
    };
  }
}

#endif  //  REVOLVEBRAIN_BRAIN_CONTROLLER_LAYEREDEXTENDEDANN_H_
