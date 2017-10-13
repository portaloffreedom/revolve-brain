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
* Description: Milan Jelisavcic
* Author: March 28, 2016
*
*/

#ifndef REVOLVEBRAIN_BRAIN_LEARNER_RLPOWERLEARNER_H_
#define REVOLVEBRAIN_BRAIN_LEARNER_RLPOWERLEARNER_H_

#include <cmath>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include <boost/thread/mutex.hpp>

#include "Learner.h"

namespace revolve
{
  namespace brain
  {
    typedef std::vector< double > Spline;

    typedef std::vector< Spline > Policy;

    typedef std::shared_ptr< Policy > PolicyPtr;

    class RLPowerLearner
            : public Learner< PolicyPtr >
    {
      public:
      struct Config;

      /// \brief The RLPower constructor reads out configuration file,
      /// deretmines which algorithm type to apply and
      /// initialises new policy.
      /// \param _modelName: name of a robot
      /// \param _brain: configuration file
      /// \param evaluator: pointer to fitness evaluatior
      /// \param n_actuators: number of actuators
      /// \param n_sensors: number of sensors
      /// \return pointer to the RLPower class object
      RLPowerLearner(std::string &_modelName,
                     Config _brain,
                     size_t _numWeightVectors);

      virtual ~RLPowerLearner();

      /// \brief = 1000; // max number of evaluations
      static const size_t MAX_EVALUATIONS;

      /// \brief = 10; // max length of policies vector
      static const size_t MAX_RANKED_POLICIES;

      /// \brief = 100; // number of data points for the interpolation cache
      static const size_t INTERPOLATION_CACHE_SIZE;

      /// \brief = 3; // number of initially sampled spline points
      static const size_t INITIAL_SPLINE_SIZE;

      /// \brief = 100; // after # generations, it increases the number of
      /// spline points
      static const size_t UPDATE_STEP;

      /// \brief // = 30.0; evaluation time for each policy
      static const double EVALUATION_RATE;

      /// \brief = 0.8; // starting value for sigma
      static const double SIGMA_START_VALUE;

      /// \brief = 0.2;
      static const double SIGMA_TAU_CORRECTION;

      /// \brief = 5; // seconds
      static const double CYCLE_LENGTH;

      /// \brief = 0.98; // sigma decay
      static const double SIGMA_DECAY_SQUARED;

      struct Config
      {
        std::string algorithm_type;
        size_t interpolation_spline_size;
        double evaluation_rate;
        size_t max_evaluations;
        size_t max_ranked_policies;
        double noise_sigma;
        double sigma_tau_correction;
        size_t source_y_size;
        size_t update_step;
        std::string policy_load_path;
      };

      protected:
      /// \brief Ranked list of used splines
      class PolicySave
      {
        public:
        PolicySave(double _fitness,
                   PolicyPtr &_p)
                : policy_(_p)
                , fitness_(_fitness)
        {
        }

        bool operator>(const PolicySave &_ps) const
        {
          return this->fitness_ > _ps.fitness_;
        }

        PolicyPtr policy_;

        double fitness_;
      };


      private:
      /// \brief Generate new policy
      void GenerateInitPolicy();

      /// \brief
      virtual void reportFitness(
              const std::string &_id,
              PolicyPtr _genotype,
              const double _fitness);

      /// \brief
      virtual PolicyPtr currentGenotype();

      /// \brief Load saved policy from JSON file
      void LoadPolicy(const std::string &_policyPath);

      /// \brief Generate interpolated spline based on number of sampled control
      /// points in 'source_y'
      /// \param _sourceY: set of control points over which interpolation is
      /// generated
      /// \param _destinationY: set of interpolated control points
      /// (default 100 points)
      void InterpolateCubic(Policy *const _sourceY,
                            Policy *_destinationY);

      /// \brief Writes current spline to file
      void LogCurrentSpline();

      /// \brief Writes best 10 splines to file
      void LogBestSplines();

      /// \brief Increment number of sampling points for policy
      void IncreaseSplinePoints();

      /// \brief Randomly select two policies and return the one with higher
      /// fitness
      /// \return an iterator from 'ranked_policies_' map
      std::map< double, PolicyPtr >::iterator BinarySelection();

      /// \brief Pointer to the current policy
      PolicyPtr currentPolicy_ = NULL;

      /// \brief Number of 'interpolation_cache_' sample points
      size_t numInterpolationPoints_;

      /// \brief Number of current generation
      size_t generationCounter_;

      /// \brief Maximal number of stored ranked policies
      size_t maxRankedPolicies_;

      /// \brief Maximal number of evaluations
      size_t maxEvaluations_;

      /// \brief Number of actuators
      size_t numActuators_;

      /// \brief
      size_t numSteps_;

      /// \brief
      size_t stepRate_;

      /// \brief Number of evaluations after which sampling size increases
      size_t updateStep_;

      /// \brief Noise in generatePolicy() function
      double sigma_;

      /// \brief Tau deviation for self-adaptive sigma
      double tau_;

      /// \brief Name of the robot
      std::string robotName_;

      /// \brief Type of the used algorithm
      std::string algorithmType_;

      /// \brief Load path for previously saved policies
      std::string policyLoadPath_;

      /// \brief Container for best ranked policies
      std::map< double, PolicyPtr, std::greater< double>> rankedPolicies_;
    };
  }
}

#endif  //  REVOLVEBRAIN_BRAIN_LEARNER_RLPOWERLEARNER_H_
