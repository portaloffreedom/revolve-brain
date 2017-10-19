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

#include <fstream>
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

#include "RafCPGController.h"

namespace revolve
{
  namespace brain
  {
    RafCPGController::RafCPGController(
            const std::string &_name,
            CPPNConfigPtr _config,
            const std::vector< ActuatorPtr > &_actuators,
            const std::vector< SensorPtr > &_sensors
    )
            : name_(_name)
            , allNeurons_(_config->allNeurons_)
            , inputNeurons_(_config->inputNeurons_)
            , outputNeurons_(_config->outputNeurons_)
            , hiddenNeurons_(_config->hiddenNeurons_)
            , outputPositionMap_(_config->outputPositionMap_)
            , inputPositionMap_(_config->inputPositionMap_)
            , idToNeuron_(_config->idToNeuron_)
            , connections_(_config->connections_)
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

    RafCPGController::~RafCPGController()
    {
      delete[] inputs_;
      delete[] outputs_;
    }

    void RafCPGController::update(
            const std::vector< ActuatorPtr > &actuators,
            const std::vector< SensorPtr > &sensors,
            double t,
            double step)
    {
      // boost::mutex::scoped_lock lock(networkMutex_);

      // Read sensor data into the input buffer
      size_t p = 0;
      for (const auto &sensor : sensors)
      {
        sensor->read(&inputs_[p]);
        p += sensor->inputs();
      }

      // Feed inputs into the input neurons
      for (const auto &inNeuron : inputNeurons_)
      {
        size_t pos = inputPositionMap_[inNeuron];
        inNeuron->SetInput(inputs_[pos]);
      }
      // Calculate new states of all neurons
      for (const auto &neuron : allNeurons_)
      {
        neuron->Update(t);
      }
      // Flip states of all neurons
      for (const auto &neuron : allNeurons_)
      {
        neuron->FlipState();
      }

      for (const auto &outNeuron : outputNeurons_)
      {
        size_t pos = outputPositionMap_[outNeuron];
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

    CPPNConfigPtr RafCPGController::getPhenotype()
    {
      boost::shared_ptr< CPPNConfig > config(new CPPNConfig());
      config->allNeurons_ = allNeurons_;
      config->inputNeurons_ = inputNeurons_;
      config->outputNeurons_ = outputNeurons_;
      config->hiddenNeurons_ = hiddenNeurons_;
      config->outputPositionMap_ = outputPositionMap_;
      config->inputPositionMap_ = inputPositionMap_;
      config->idToNeuron_ = idToNeuron_;
      config->connections_ = connections_;

      return config;
    }

    void RafCPGController::setPhenotype(CPPNConfigPtr _config)
    {
      allNeurons_ = _config->allNeurons_;
      for (NeuronPtr neuron: allNeurons_)
      {
        neuron->reset();
      }
      inputNeurons_ = _config->inputNeurons_;
      outputNeurons_ = _config->outputNeurons_;
      hiddenNeurons_ = _config->hiddenNeurons_;
      outputPositionMap_ = _config->outputPositionMap_;
      inputPositionMap_ = _config->inputPositionMap_;
      idToNeuron_ = _config->idToNeuron_;
      connections_ = _config->connections_;
    }

    void RafCPGController::writeNetwork(std::ofstream &write_to)
    {
      boost::adjacency_list<> graph(allNeurons_.size());
      for (size_t i = 0; i < allNeurons_.size(); i++)
      {
        auto connectionsToAdd = allNeurons_[i]->IncomingConnections();
        for (const auto &connectionToAdd : connectionsToAdd)
        {
          auto input = connectionToAdd.second->GetInputNeuron();
          long indexInput = std::find(allNeurons_.begin(),
                                      allNeurons_.end(),
                                      input) - allNeurons_.begin();
          boost::add_edge(indexInput, i, graph);
        }
      }
      auto *names = new std::string[allNeurons_.size()];
      for (size_t i = 0; i < allNeurons_.size(); ++i)
      {
        std::stringstream nodeName;
        nodeName << allNeurons_[i]->Id() + " of type: "
                    + allNeurons_[i]->Type() << std::endl;
        for (std::pair< std::string, double >
                  param : allNeurons_[i]->Parameters())
        {
          nodeName << param.first << ": " << param.second << std::endl;
        }
        names[i] = nodeName.str();
      }

      boost::write_graphviz(write_to, graph, boost::make_label_writer(names));
      delete[] names;
    }
  }
}
