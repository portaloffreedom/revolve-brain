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
* Description: Brain class for the default Neural Network as specified by
*              Revolve. This is loosely based on the neural network
*              code provided with the Robogen framework.
* Author: Elte Hupkes
*
*/

#ifndef REVOLVEBRAIN_BRAIN_NEURALNETWORK_H_
#define REVOLVEBRAIN_BRAIN_NEURALNETWORK_H_

#include <map>
#include <string>
#include <vector>

#include <boost/thread/mutex.hpp>

#include "Brain.h"

// These numbers are quite arbitrary. It used to be in:13 out:8
// for the Arduino, but I upped them both to 20 to accomodate other
// scenarios. Should really be enforced in the Python code, this
// implementation should not be the limit.
#define MAX_INPUT_NEURONS 20
#define MAX_OUTPUT_NEURONS 20

// Arbitrary value
#define MAX_HIDDEN_NEURONS 30

// Convenience
#define MAX_NON_INPUT_NEURONS (MAX_INPUT_NEURONS + MAX_OUTPUT_NEURONS)

// (bias, tau, gain) or (phase offset, period, gain)
#define MAX_NEURON_PARAMS 3

namespace revolve
{
  namespace brain
  {
    /// \brief Copied from NeuronRepresentation.h
    enum neuronType
    {
      INPUT,
      SIMPLE,
      SIGMOID,
      CTRNN_SIGMOID,
      OSCILLATOR,
      SUPG
    };

    class NeuralNetwork
            : public Brain
    {
      public:
      /// \brief
      NeuralNetwork();

      /// \param Name of the robot
      /// \param The brain node
      /// \param Reference to motor list, which might be reordered
      /// \param Reference to the sensor list, which might be reordered
      NeuralNetwork(std::string modelName,
                    std::vector<ActuatorPtr> &actuators,
                    std::vector<SensorPtr> &sensors);

      /// \brief
      virtual ~NeuralNetwork() override;


      /// \param Motor list
      /// \param Sensor list
      virtual void update(const std::vector<ActuatorPtr> &actuators,
                          const std::vector<SensorPtr> &sensors,
                          double t,
                          double step) override;

      protected:
      /// \brief Steps the neural network
      void step(double time);

      /// \brief Mutex for stepping / updating the network
      boost::mutex networkMutex_;

      /// \brief Connection weights, separated into three arrays for
      /// convenience. Note that only output and hidden neurons are weight
      /// targets. Weights are stored with gaps, meaning that every neuron holds
      /// entries for the maximum possible number of connections. This makes
      /// restructuring the weights arrays when a hidden neuron is removed
      /// slightly less cumbersome.
      double inputWeights_[
              MAX_INPUT_NEURONS * (MAX_OUTPUT_NEURONS + MAX_HIDDEN_NEURONS)];
      double outputWeights_[
              MAX_OUTPUT_NEURONS * (MAX_OUTPUT_NEURONS + MAX_HIDDEN_NEURONS)];
      double hiddenWeights_[
              MAX_HIDDEN_NEURONS * (MAX_OUTPUT_NEURONS + MAX_HIDDEN_NEURONS)];

      // Unlike weights, types, params and current states are stored without
      // gaps, meaning the first `m` entries are for output neurons, followed
      // by `n` entries for hidden neurons. If a hidden neuron is removed,
      // the items beyond it are moved back.

      /// \brief  Type of each non-input neuron
      size_t types_[(MAX_OUTPUT_NEURONS + MAX_HIDDEN_NEURONS)];

      /// \brief  Params for hidden and output neurons, quantity depends on the
      /// type of neuron
      double params_[
              MAX_NEURON_PARAMS * (MAX_OUTPUT_NEURONS + MAX_HIDDEN_NEURONS)];

      /// \brief  Output states arrays for the current state and the next state.
      double state1_[MAX_OUTPUT_NEURONS + MAX_HIDDEN_NEURONS];
      double state2_[MAX_OUTPUT_NEURONS + MAX_HIDDEN_NEURONS];

      /// \brief  One input state for each input neuron
      double input_[MAX_INPUT_NEURONS];

      /// \brief  Used to determine the current state array. False = state1,
      /// true = state2.
      bool flipState_;

      /// \brief  Stores the type of each neuron ID
      std::map<std::string, std::string> layerMap_;

      /// \brief  Stores the position of each neuron ID, relative to its type
      std::map<std::string, size_t> positionMap_;

      /// \brief  The number of inputs
      size_t nInputs_;

      /// \brief  The number of outputs
      size_t nOutputs_;

      /// \brief  The number of hidden units
      size_t nHidden_;

      /// \brief  The number of non-inputs (i.e. nOutputs + nHidden)
      size_t nNonInputs_;
    };
  }
}


#endif  //  REVOLVEBRAIN_BRAIN_NEURALNETWORK_H_
