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
#include <string>
#include <vector>

#include "Evaluator.h"
#include "SplitBrain.h"

namespace revolve
{
  namespace brain
  {
    template < typename Phenotype, typename Genotype >
    class ConverterSplitBrain
            : public SplitBrain< Phenotype, Genotype >
    {
      public:
      /// \brief
      ConverterSplitBrain(
              Phenotype (*convertForController)(Genotype),
              Genotype (*convertForLearner)(Phenotype),
              const std::string model_name
      )
              : name_(model_name)
              , isFirstRun_(true)
              , numRuns_(0)
              , convertForController_(convertForController)
              , convertForLearner_(convertForLearner)
      {}

      /// \brief
      virtual ~ConverterSplitBrain()
      {}

      /// \brief Update step called for the brain.
      /// \param actuators List of actuators
      /// \param sensors List of sensors
      /// \param t Current simulation time
      /// \param step Actuation step size in seconds
      virtual void update(
              const std::vector< ActuatorPtr > &actuators,
              const std::vector< SensorPtr > &sensors,
              double t,
              double step)
      {
        if (isFirstRun_)
        {
          this->controller_->setPhenotype(convertForController_(
                  this->learner_->currentGenotype()));

          startTime_ = t;
          evaluator_->start();
          isFirstRun_ = false;
        }

        // and generation_counter_ < max_evaluations_) {
        if ((t - startTime_) > evaluationRate_)
        {
          double fitness = evaluator_->fitness();
          writeCurrent(fitness);

          this->learner_->reportFitness(
                  name_,
                  convertForLearner_(this->controller_->getPhenotype()),
                  fitness);

          Phenotype controllerPhenotype = convertForController_(
                  this->learner_->currentGenotype());

          this->controller_->setPhenotype(controllerPhenotype);
          startTime_ = t;
          numGeneration_++;
          evaluator_->start();
        }
        this->controller_->update(actuators, sensors, t, step);
      }

      /// \brief
      void writeCurrent(const double _fitness)
      {
        std::ofstream outputFile;
        outputFile.open(name_ + ".log",
                        std::ios::app | std::ios::out | std::ios::ate);
        outputFile << "- generation: " << numGeneration_ << std::endl;
        outputFile << "  velocity: " << _fitness << std::endl;
        // TODO: Should we record an entire generation?
        outputFile.close();
      }

      protected:
      /// \brief
      std::string name_;

      /// \brief
      bool isFirstRun_;

      /// \brief
      int numGeneration_ = 0;

      /// \brief
      int numRuns_;

      /// \brief
      double startTime_ = 0;

      /// \brief
      double evaluationRate_ = 30.0;

      /// \brief
      EvaluatorPtr evaluator_;

      /// \brief
      Phenotype (*convertForController_)(Genotype);

      /// \brief
      Genotype (*convertForLearner_)(Phenotype);
    };
  }
}

#endif  //  REVOLVEBRAIN_BRAIN_CONVERTINGSPLITBRAIN_H_
