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

//
// Created by matteo on 3/13/17.
//

#ifndef TRIANGLEOFLIFE_CPGCONTROLLER_H
#define TRIANGLEOFLIFE_CPGCONTROLLER_H

#include "brain/cpg/CPGNetwork.h"
#include "BaseController.h"

namespace revolve { namespace brain {

    class CPGController : public BaseController {
    public:
        CPGController(size_t n_inputs, size_t n_outputs);

        virtual ~CPGController();

        void update(const std::vector<ActuatorPtr> &actuators,
                    const std::vector<SensorPtr> &sensors,
                    double t, double step) override;

        std::vector<cpg::CPGNetwork *>::iterator beginCPGNetwork()
        { return cpgs.begin(); }

        std::vector<cpg::CPGNetwork *>::iterator endCPGNetwork()
        { return cpgs.end(); }

    protected:
        void initRandom(float sigma);

    protected:
        size_t n_inputs, n_outputs;

        //list of cpgs
        std::vector<cpg::CPGNetwork *> cpgs;

        /** Connection matrix between the different servos
         * First is start of the connections, second is end.
         * Example: connections[0][1].we is the connection starting from servo 0
         * and reacing servo 1 for the RythmGenerationNeurons E
         */
        std::vector<std::vector<cpg::CPGNetwork::Weights>> connections;

        // CACHING VECTORS
        double *inputs_vector,
            *outputs_vector;
    };

}}


#endif  // TRIANGLEOFLIFE_CPGCONTROLLER_H
