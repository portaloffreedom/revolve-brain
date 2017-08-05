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

#ifndef TRIANGLEOFLIFE_CPGCONTROLLER_H
#define TRIANGLEOFLIFE_CPGCONTROLLER_H

#include <vector>

#include "BaseController.h"
#include "brain/cpg/CPGNetwork.h"

namespace revolve
{
  namespace brain
  {
    class CPGController
            : public BaseController
    {
      public:
      /// \brief
      CPGController(size_t n_inputs,
                    size_t n_outputs);

      /// \brief
      virtual ~CPGController();

      /// \brief
      void update(const std::vector<ActuatorPtr> &actuators,
                  const std::vector<SensorPtr> &sensors,
                  double t,
                  double step) override;

      /// \brief
      std::vector<cpg::CPGNetwork *>::iterator beginCPGNetwork()
      {
        return cpgs.begin();
      }

      /// \brief
      std::vector<cpg::CPGNetwork *>::iterator endCPGNetwork()
      {
        return cpgs.end();
      }

      protected:
      /// \brief
      void initRandom(float sigma);

      protected:
      /// \brief
      size_t n_inputs, n_outputs;

      /// \brieflist of cpgs
      std::vector<cpg::CPGNetwork *> cpgs;

      /// \brief Connection matrix between the different servos
      /// First is start of the connections, second is end.
      /// Example: connections[0][1].we is the connection starting from servo 0
      /// and reacing servo 1 for the RythmGenerationNeurons E
      std::vector<std::vector<cpg::CPGNetwork::Weights>> connections;

      /// \brief CACHING VECTORS
      double *inputs_vector,
              *outputs_vector;
    };
  }
}


#endif  // TRIANGLEOFLIFE_CPGCONTROLLER_H
