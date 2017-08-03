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

#ifndef REVOLVEBRAIN_BRAIN_CONVERTINGSPLITBRAIN_H_
#define REVOLVEBRAIN_BRAIN_CONVERTINGSPLITBRAIN_H_

#include <iostream>
#include <fstream>

#include "Evaluator.h"
#include "SplitBrain.h"

namespace revolve {
namespace brain {


template <typename Phenotype, typename Genotype>
class ConverterSplitBrain
        : public SplitBrain<Phenotype, Genotype>
{
public:
    ConverterSplitBrain(Phenotype (*convertForController)(Genotype),
                        Genotype (*convertForLearner)(Phenotype),
                        const std::string model_name)
            : model_name_(model_name)
            , is_first_run_(true)
            , run_count_(0)
            , convertForController_(convertForController)
            , convertForLearner_(convertForLearner)
    {};

    virtual ~ConverterSplitBrain()
    {};

    /**
     * Update step called for the brain.
     /// \param actuators List of actuators
     /// \param sensors List of sensors
     /// \param t Current simulation time
     /// \param step Actuation step size in seconds
     */
    virtual void update(const std::vector<ActuatorPtr> &actuators,
                        const std::vector<SensorPtr> &sensors,
                        double t,
                        double step)
    {
      if (is_first_run_) {
        this->controller_->setPhenotype(
                convertForController_(this->learner_->currentGenotype())
        );

        start_eval_time_ = t;
        evaluator_->start();
        is_first_run_ = false;
      }

      // && generation_counter_ < max_evaluations_) {
      if ((t - start_eval_time_) > evaluation_rate_) {
        double fitness = evaluator_->fitness();
        writeCurrent(fitness);

        this->learner_->reportFitness(
                model_name_,
                convertForLearner_(this->controller_->getPhenotype()),
                fitness
        );

        Phenotype controllerPhenotype = convertForController_(
                this->learner_->currentGenotype()
        );

        this->controller_->setPhenotype(controllerPhenotype);
        start_eval_time_ = t;
        generation_counter_++;
        evaluator_->start();
      }
      this->controller_->update(actuators, sensors, t, step);
    }

    void writeCurrent(double fitness)
    {
      std::ofstream outputFile;
      outputFile.open(model_name_ + ".log",
                      std::ios::app | std::ios::out | std::ios::ate);
      outputFile << "- generation: " << generation_counter_ << std::endl;
      outputFile << "  velocity: " << fitness << std::endl;
      // TODO: Should we record an entire generation?
      outputFile.close();
    }

protected:

    std::string model_name_;

    bool is_first_run_;

    int generation_counter_ = 0;
    int run_count_;

    double start_eval_time_ = 0;
    double evaluation_rate_ = 30.0;

    EvaluatorPtr evaluator_;

    Phenotype (*convertForController_)(Genotype);

    Genotype (*convertForLearner_)(Phenotype);

//private:
//    using Brain::update;
};

}
}

#endif // REVOLVEBRAIN_BRAIN_CONVERTINGSPLITBRAIN_H_
