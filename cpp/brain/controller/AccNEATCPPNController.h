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
* Author: Matteo De Carlo
* Date: March 13, 2017
*
*/

#ifndef REVOLVE_BRAIN_ACCNEATCPPNCONTROLLER_H
#define REVOLVE_BRAIN_ACCNEATCPPNCONTROLLER_H

#include <vector>

#include "BaseController.h"
#include "network/cpu/cpunetwork.h"

namespace revolve
{
  namespace brain
  {
    class AccNEATCPPNController
            : public BaseController
    {
      public:
      /// \brief
      AccNEATCPPNController(
              const size_t n_inputs,
              const size_t n_outputs);

      /// \brief
      virtual ~AccNEATCPPNController();

      /// \brief Update the controller to the next step and sends signals
      /// to the actuators
      /// \param actuators outputs of the controller
      /// \param sensors inputs of the controller
      /// \param t global time reference
      /// \param step time since last update
      virtual void update(const std::vector<ActuatorPtr> &actuators,
                          const std::vector<SensorPtr> &sensors,
                          double t,
                          double step) override;

      /// \brief
      void activate(const double *const inputs);

      /// \brief
      NEAT::real_t *getOutputs()
      {
        return cppn->get_outputs();
      }

      /// \brief
      void setCPPN(NEAT::CpuNetwork *cppn);

      protected:
      /// \brief
      NEAT::CpuNetwork *cppn;

      /// \brief
      const size_t n_inputs, n_outputs;

      private:
      /// \brief
      double *inputs_vector;

      /// \brief
      double *outputs_vector;
    };
  }
}

#endif  // REVOLVE_BRAIN_ACCNEATCPPNCONTROLLER_H
