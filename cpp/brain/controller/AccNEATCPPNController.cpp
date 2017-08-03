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

#include "AccNEATCPPNController.h"


using namespace revolve::brain;


AccNEATCPPNController::AccNEATCPPNController(size_t n_inputs,
                                             size_t n_outputs)
    : n_inputs(n_inputs), n_outputs(n_outputs)
{
  inputs_vector = new double[n_inputs];
  outputs_vector = new double[n_outputs];
}

AccNEATCPPNController::~AccNEATCPPNController()
{
  delete[] inputs_vector;
  delete[] outputs_vector;
}

void AccNEATCPPNController::update(const std::vector<ActuatorPtr> &actuators,
                                   const std::vector<SensorPtr> &sensors,
                                   double t, double step)
{
  // Read sensor data
  unsigned int p = 0;
  for (auto sensor : sensors) {
    sensor->read(&inputs_vector[p]);
    p += sensor->inputs();
  }
  assert(p == n_inputs);

  this->activate(inputs_vector);

  NEAT::real_t *outputs = getOutputs();
  for (size_t i=0; i<n_outputs; i++) {
    outputs_vector[i] = outputs[i];
  }

  // send signals to actuators
  p = 0;
  for (auto actuator: actuators) {
    actuator->update(&outputs_vector[p],
                     step);
    p += actuator->outputs();
  }
  assert(p == n_outputs);
}

#define NCYCLES 1
void AccNEATCPPNController::activate(const double *const inputs)
{
  for (size_t i = 0; i < n_inputs; i++) {
    cppn->load_sensor(i, (NEAT::real_t) inputs[i]);
  }

  cppn->activate(NCYCLES);
}

void AccNEATCPPNController::setCPPN(NEAT::CpuNetwork *cppn)
{
  AccNEATCPPNController::cppn = cppn;
}
