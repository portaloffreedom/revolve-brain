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

#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

#include "LayeredExtCPPN.h"

namespace revolve
{
  namespace brain
  {
    LayeredExtNNController::LayeredExtNNController(
            const std::string &_name,
            boost::shared_ptr< LayeredExtNNConfig > Config,
            const std::vector< ActuatorPtr > &_actuators,
            const std::vector< SensorPtr > &_sensors
    )
            : modelName_(_name)
            , layers_(Config->layers_)
            , outputPositionMap_(Config->outputPositionMap_)
            , inputPositionMap_(Config->inputPositionMap_)
            , idToNeuron_(Config->idToNeuron_)
            , connections_(Config->connections_)
    {
      size_t p = 0;
      for (const auto &sensor : _sensors)
      {
        p += sensor->inputs();
      }
      inputs_ = new double[p];
      p = 0;
      for (const auto &actuator : _actuators)
      {
        p += actuator->outputs();
      }
      outputs_ = new double[p];
    }


    LayeredExtNNController::~LayeredExtNNController()
    {
      delete[] inputs_;
      delete[] outputs_;
    }

    void LayeredExtNNController::update(
            const std::vector< ActuatorPtr > &actuators,
            const std::vector< SensorPtr > &sensors,
            double t,
            double step)
    {
      // Read sensor data into the input buffer
      size_t p = 0;
      for (const auto &sensor : sensors)
      {
        sensor->read(&inputs_[p]);
        p += sensor->inputs();
      }

      // Feed inputs into the input neurons
      for (const auto &inNeuron : layers_[0])
      {
        auto pos = inputPositionMap_[inNeuron];
        inNeuron->SetInput(inputs_[pos]);
      }

      // Calculate new states of all neurons
      for (std::vector< NeuronPtr > layer : layers_)
      {
        for (const auto &neuron : layer)
        {
          neuron->Update(t);
        }
        for (const auto &neuron : layer)
        {
          neuron->FlipState();
        }
      }

      for (const auto &outNeuron : layers_[layers_.size() - 1])
      {
        auto pos = outputPositionMap_[outNeuron];
        outputs_[pos] = outNeuron->Output();
      }

      // Send new signals to the actuators
      p = 0;
      for (const auto &actuator: actuators)
      {
        actuator->update(&outputs_[p], step);
        p += actuator->outputs();
      }
    }

    boost::shared_ptr< LayeredExtNNConfig >
    LayeredExtNNController::getGenotype()
    {
      boost::shared_ptr< LayeredExtNNConfig > Config(new LayeredExtNNConfig());
      Config->layers_ = layers_;
      Config->outputPositionMap_ = outputPositionMap_;
      Config->inputPositionMap_ = inputPositionMap_;
      Config->idToNeuron_ = idToNeuron_;
      Config->connections_ = connections_;
      return Config;
    }

    void LayeredExtNNController::setGenotype(
            boost::shared_ptr< LayeredExtNNConfig > Config)
    {
      layers_ = Config->layers_;
      outputPositionMap_ = Config->outputPositionMap_;
      inputPositionMap_ = Config->inputPositionMap_;
      idToNeuron_ = Config->idToNeuron_;
      connections_ = Config->connections_;
    }

    void LayeredExtNNController::writeNetwork(std::ofstream &write_to)
    {
      std::vector< NeuronPtr > allNeurons_;
      for (const auto &v : layers_)
      {
        allNeurons_.insert(allNeurons_.end(), v.begin(), v.end());
      }
      boost::adjacency_list<> graph(allNeurons_.size());
      for (size_t i = 0; i < allNeurons_.size(); i++)
      {
        auto connectionsToAdd = allNeurons_[i]->IncomingConnections();
        for (const auto &connectionToAdd : connectionsToAdd)
        {
          auto input = connectionToAdd.second->GetInputNeuron();
          auto indexInput = std::find(
                  allNeurons_.begin(),
                  allNeurons_.end(),
                  input) - allNeurons_.begin();
          boost::add_edge(indexInput, i, graph);
        }
      }
      auto *names = new std::string[allNeurons_.size()];
      size_t i = 0;
      for (const auto &neuron : allNeurons_)
      {
        std::stringstream nodeName;
        nodeName << neuron->Id() + " of type: " + neuron->Type()
                << std::endl;
        for (const auto &param : neuron->Parameters())
        {
          nodeName << param.first << ": " << param.second << std::endl;
        }
        names[i++] = nodeName.str();
      }

      boost::write_graphviz(write_to, graph, boost::make_label_writer(names));
      delete[] names;
    }
  }
}
