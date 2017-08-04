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

#ifndef REVOLVE_BRAIN_SUPGBRAIN_H
#define REVOLVE_BRAIN_SUPGBRAIN_H

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Brain.h"
#include "Evaluator.h"

#include "neat/AsyncNEAT.h"
#include "supg/SUPGNeuron.h"

namespace revolve
{
  namespace brain
  {
    class SUPGBrain
            : public Brain
    {
      // METHODS
      public:
      SUPGBrain(const std::string &robot_name,
                EvaluatorPtr evaluator,
                const std::vector<std::vector<float> > &neuron_coordinates,
                const std::vector<ActuatorPtr> &actuators,
                const std::vector<SensorPtr> &sensors);

      ~SUPGBrain()
      {}

      using Brain::update;

      virtual void update(const std::vector<ActuatorPtr> &actuators,
                          const std::vector<SensorPtr> &sensors,
                          double t,
                          double step) override;

      protected:
      SUPGBrain(EvaluatorPtr evaluator);

      //// Templates ---------------------------------------------------------

      template <typename ActuatorContainer, typename SensorContainer>
      void controller(const ActuatorContainer &actuators,
                      const SensorContainer &sensors,
                      double t,
                      double step)
      {
        assert(n_outputs == actuators.size());

        // Read sensor data and feed the neural network
        double *inputs = new double[n_inputs];
        size_t p = 0;
        for (auto sensor : sensors)
        {
          sensor->read(&inputs[p]);
          p += sensor->inputs();
        }
        assert(p == n_inputs);

        // load sensors
        for (size_t i = 0; i < n_inputs; i++)
        {
          neurons[0]->load_sensor(i, inputs[i]);
        }

        // Activate network and save results
        double *outputs = new double[n_outputs];
        for (size_t i = 0; i < neurons.size(); i++)
        {
          neurons[i]->activate(t);
          outputs[i] = neurons[i]->get_outputs()[0] * 2 - 1;
        }

        // send signals to actuators
        p = 0;
        for (auto actuator: actuators)
        {
          actuator->update(&outputs[p], step);
          p += actuator->outputs();
        }
        assert(p == n_outputs);

        delete[] inputs;
      }

      template <typename ActuatorContainer, typename SensorContainer>
      void update(const ActuatorContainer &actuators,
                  const SensorContainer &sensors,
                  double t,
                  double step)
      {
        this->learner(t);  // CAREFUL, virtual function
        controller<ActuatorContainer, SensorContainer>
                (actuators, sensors, t, step);
      }

      /// \brief
      void init_async_neat();

      protected:
      /// \brief
      virtual void learner(double t);

      /// \brief
      virtual double getFitness();

      /// \brief
      void nextBrain();

      /// \brief
      static long GetMAX_EVALUATIONSenv();

      /// \brief
      static double GetFREQUENCY_RATEenv();

      /// \brief
      static double GetCYCLE_LENGTHenv();

      /// \brief
      static const char *getVARenv(const char *var_name);

      // DATA
      protected:
      /// \brief
      size_t n_inputs, n_outputs;

      /// \brief
      std::vector<std::vector<float> > neuron_coordinates;

      /// \brief
      std::unique_ptr<AsyncNeat> neat;

      /// \brief
      EvaluatorPtr evaluator;

      /// \brief
      const std::string robot_name;

      /// \brief
      double start_eval_time;

      /// \brief
      long generation_counter;

      /// \brief
      std::shared_ptr<NeatEvaluation> current_evalaution;

      /// \brief
      std::vector<std::unique_ptr<SUPGNeuron> > neurons;

      /// \brief Number of evaluations before the program quits. Usefull to do
      /// long run tests. If negative (default value), it will never stop.
      ///
      /// Takes value from env variable SUPG_MAX_EVALUATIONS.
      /// Default value -1  // max number of evaluations
      const long MAX_EVALUATIONS;

      /// \brief  How long should an evaluation lasts (in seconds)
      ///
      /// Takes value from env variable SUPG_FREQUENCY_RATE
      /// Default value 30 seconds//= 30; // seconds
      const double FREQUENCY_RATE;

      /// \brief  How long should the supg timer cicle be (in seconds)
      ///
      /// Takes value from env variable SUPG_CYCLE_LENGTH
      /// Default value 5 seconds // = 5; // seconds
      const double CYCLE_LENGTH;
    };
  }
}

#endif  //  REVOLVE_BRAIN_SUPGBRAIN_H
