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

#ifndef REVOLVEBRAIN_BRAIN_CONTROLLER_EXTENDEDANN_H_
#define REVOLVEBRAIN_BRAIN_CONTROLLER_EXTENDEDANN_H_

#include <map>
#include <vector>
#include <string>

#include "Controller.h"

#include "brain/Types.h"
#include "brain/Evaluator.h"
#include "brain/controller/extnn/BiasNeuron.h"
#include "brain/controller/extnn/DifferentialCPG.h"
#include "brain/controller/extnn/ENeuron.h"
#include "brain/controller/extnn/InputNeuron.h"
#include "brain/controller/extnn/InputDependentOscillatorNeuron.h"
#include "brain/controller/extnn/LeakyIntegrator.h"
#include "brain/controller/extnn/LinearNeuron.h"
#include "brain/controller/extnn/NeuralConnection.h"
#include "brain/controller/extnn/OscillatorNeuron.h"
#include "brain/controller/extnn/RythmGenerationCPG.h"
#include "brain/controller/extnn/SigmoidNeuron.h"
#include "brain/controller/extnn/VOscillator.h"
#include "brain/controller/extnn/XOscillator.h"

namespace revolve
{
  namespace brain
  {

    struct CPPNConfig
    {
      //vector containing all neurons
      std::vector<NeuronPtr> allNeurons_;
      //vector containing the input neurons
      std::vector<NeuronPtr> inputNeurons_;
      //vector containing the output neurons
      std::vector<NeuronPtr> outputNeurons_;
      //vector containing the hidden neurons
      std::vector<NeuronPtr> hiddenNeurons_;
      // positions for indexing into the outputs_ buffer for each output neuron
      std::map<NeuronPtr, int> outputPositionMap_;
      // positions for indexing into the inputs_ buffer for each input neuron
      std::map<NeuronPtr, int> inputPositionMap_;
      // Map neuron id strings to Neuron objects
      std::map<std::string, NeuronPtr> idToNeuron_;
      //vector of all the neural connections
      std::vector<NeuralConnectionPtr> connections_;
    };

    /// \brief extended neural network controller usable with standard neat or
    /// hyper neat (use different conversion methods)
    class RafCPGController
            : public Controller<CPPNConfigPtr>
    {
      public:
      /// \brief Constructor for a neural network including neurons that are of
      /// a different type than the usual ones.
      /// \param model_name: name of the model
      /// \param _config: configuration file
      /// \param evaluator: pointer to the evaluator that is used
      /// \param actuators: vector list of robot's actuators
      /// \param sensors: vector list of robot's sensors
      /// \return pointer to the neural network
      RafCPGController(std::string model_name,
                       CPPNConfigPtr _config,
                       const std::vector<ActuatorPtr> &actuators,
                       const std::vector<SensorPtr> &sensors);

      virtual ~RafCPGController();

      /**
      * Method for updating sensors readings, actuators positions
      /// \param actuators: vector list of robot's actuators
      /// \param sensors: vector list of robot's sensors
      /// \param t: current time
      /// \param step:
      */
      virtual void update(const std::vector<ActuatorPtr> &actuators,
                          const std::vector<SensorPtr> &sensors,
                          double t,
                          double step);

      /**
       * Gets the weight of all the connections
       /// \return weights of all neural connections
       */
      virtual CPPNConfigPtr getPhenotype();

      /**
       * Changes the weights of the neural connections
       /// \param weights: new weights to be assigned
       */
      virtual void setPhenotype(CPPNConfigPtr _config);

      void writeNetwork(std::ofstream &write_to);

      protected:
      /// \brief name of the robot
      std::string modelName_;

      /// \brief buffer of input values from the sensors
      double *inputs_;

      /// \brief buffer of output values for the actuators
      double *outputs_;

      /// \brief vector containing all neurons
      std::vector<NeuronPtr> allNeurons_;

      /// \brief vector containing the input neurons
      std::vector<NeuronPtr> inputNeurons_;

      /// \brief vector containing the output neurons
      std::vector<NeuronPtr> outputNeurons_;

      /// \brief vector containing the hidden neurons
      std::vector<NeuronPtr> hiddenNeurons_;

      /// \brief positions for indexing into the outputs_ buffer for each output
      /// neuron
      std::map<NeuronPtr, int> outputPositionMap_;

      /// \brief positions for indexing into the inputs_ buffer for each input
      /// neuron
      std::map<NeuronPtr, int> inputPositionMap_;

      /// \brief Map neuron id strings to Neuron objects
      std::map<std::string, NeuronPtr> idToNeuron_;

      /// \brief vector of all the neural connections
      std::vector<NeuralConnectionPtr> connections_;

    };
  }
}

#endif // REVOLVEBRAIN_BRAIN_CONTROLLER_EXTENDEDANN_H_
