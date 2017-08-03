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

#ifndef REVOLVE_BRAIN_ACCNEATLEARNER_H
#define REVOLVE_BRAIN_ACCNEATLEARNER_H

#include "neat/AsyncNEAT.h"
#include "brain/Evaluator.h"
#include "BaseLearner.h"

namespace revolve { namespace brain {

    class AccNEATLearner : public BaseLearner {
    public: // METHODS
        AccNEATLearner(const std::string &robot_name,
                       EvaluatorPtr evaluator,
                       size_t n_inputs,
                       size_t n_outputs,
                       const float evaluationTime,
                       const long maxEvaluations = -1);

        virtual ~AccNEATLearner();


        virtual BaseController *
        update(const std::vector<SensorPtr> &sensors,
               double t,
               double step) override;

    protected:
        virtual BaseController * create_new_controller(double fitness) override;

        float getFitness();

    private:
        void initAsyncNeat();

    void writeCurrent(double fitness);

    protected: // VARIABLES

        EvaluatorPtr evaluator;
        size_t n_inputs, n_outputs;

        unsigned int generation_counter;
        double start_eval_time;

        std::unique_ptr<AsyncNeat> neat;
        std::shared_ptr<NeatEvaluation> current_evalaution;

        /**
         * Number of evaluations before the program quits. Usefull to do long run
         * tests. If negative (default value), it will never stop.
         *
         * Default value -1
         */
        const long MAX_EVALUATIONS; //= -1; // max number of evaluations

        /**
         * How long should an evaluation lasts (in seconds)
         *
         * 30 seconds is usually a good value
         */
        double EVALUATION_TIME;
    };

}}


#endif  // REVOLVE_BRAIN_ACCNEATLEARNER_H
