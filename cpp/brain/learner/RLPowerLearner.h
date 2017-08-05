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
    typedef std::vector<double> Spline;

    typedef std::vector<Spline> Policy;

    typedef std::shared_ptr<Policy> PolicyPtr;

    class RLPowerLearner
            : public Learner<PolicyPtr>
    {
      public:
      struct Config;

      /// \brief The RLPower constructor reads out configuration file,
      /// deretmines which algorithm type to apply and
      /// initialises new policy.
      /// \param modelName: name of a robot
      /// \param brain: configuration file
      /// \param evaluator: pointer to fitness evaluatior
      /// \param n_actuators: number of actuators
      /// \param n_sensors: number of sensors
      /// \return pointer to the RLPower class object
      RLPowerLearner(std::string modelName,
                     Config brain,
                     size_t n_weight_vectors);

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
        PolicyPtr policy_;
        double fitness_;

        PolicySave(double fitness,
                   PolicyPtr &p)
                : policy_(p)
                  , fitness_(fitness)
        {}

        bool
        operator>(const PolicySave &ps) const
        {
          return this->fitness_ > ps.fitness_;
        }
      };


      private:
      /// \brief Generate new policy
      void generateInitPolicy();

      /// \brief
      virtual void reportFitness(std::string id,
                                 PolicyPtr genotype,
                                 double fitness);

      /// \brief
      virtual PolicyPtr currentGenotype();

      /// \brief Load saved policy from JSON file
      void loadPolicy(std::string const policy_path);

      /// \brief Generate interpolated spline based on number of sampled control
      /// points in 'source_y'
      /// \param source_y: set of control points over which interpolation is
      /// generated
      /// \param destination_y: set of interpolated control points
      /// (default 100 points)
      void interpolateCubic(Policy *const source_y,
                            Policy *destination_y);

      /// \brief Writes current spline to file
      void writeCurrent();

      /// \brief Writes best 10 splines to file
      void writeElite();

      /// \brief Increment number of sampling points for policy
      void increaseSplinePoints();

      /// \brief Randomly select two policies and return the one with higher
      /// fitness
      /// \return an iterator from 'ranked_policies_' map
      std::map<double, PolicyPtr>::iterator binarySelection();

      /// \brief Pointer to the current policy
      PolicyPtr current_policy_ = NULL;

      /// \brief Number of 'interpolation_cache_' sample points
      size_t interpolation_spline_size_;

      /// \brief Number of current generation
      size_t generation_counter_;

      /// \brief Maximal number of stored ranked policies
      size_t max_ranked_policies_;

      /// \brief Maximal number of evaluations
      size_t max_evaluations_;

      /// \brief Number of actuators
      size_t n_weight_vectors_;

      /// \brief
      size_t source_y_size_;

      /// \brief
      size_t step_rate_;

      /// \brief Number of evaluations after which sampling size increases
      size_t update_step_;

      /// \brief Noise in generatePolicy() function
      double noise_sigma_;

      /// \brief Tau deviation for self-adaptive sigma
      double sigma_tau_correction_;

      /// \brief Name of the robot
      std::string robot_name_;

      /// \brief Type of the used algorithm
      std::string algorithm_type_;

      /// \brief Load path for previously saved policies
      std::string policy_load_path_;

      /// \brief Container for best ranked policies
      std::map<double, PolicyPtr, std::greater<double>> ranked_policies_;
    };
  }
}

#endif  //  REVOLVEBRAIN_BRAIN_LEARNER_RLPOWERLEARNER_H_
