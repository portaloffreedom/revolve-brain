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
* Author: Milan Jelisavcic
 * Date: March 28, 2016
*
*/

#ifndef REVOLVEBRAIN_BRAIN_RLPOWER_H_
#define REVOLVEBRAIN_BRAIN_RLPOWER_H_

#include <cmath>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include <boost/thread/mutex.hpp>

#include "Brain.h"
#include "Evaluator.h"

namespace revolve
{
  namespace brain
  {
    //    typedef std::vector<double> Spline;
    //    typedef std::vector<Spline *> Policy;
    class RLPower
            : public revolve::brain::Brain
    {
      protected:
      struct Config;
      public:
      typedef std::vector< double > Spline;
      typedef std::vector< Spline > Policy;
      typedef std::shared_ptr< Policy > PolicyPtr;

      // typedef const std::shared_ptr<revolve::msgs::ModifyNeuralNetwork const>
      // ConstModifyNeuralNetworkPtr;

      /// \brief  The RLPower constructor reads out configuration file,
      /// deretmines which algorithm type to apply and initialises new policy.
      /// \param modelName: name of a robot
      /// \param brain: configuration file
      /// \param evaluator: pointer to fitness evaluatior
      /// \param n_actuators: number of actuators
      /// \return pointer to the RLPower class object
      RLPower(
              std::string modelName,
              Config brain,
              EvaluatorPtr evaluator,
              size_t n_actuators);

      ~RLPower() override;

      using revolve::brain::Brain::update;

      /// \brief  Method for updating sensors readings, actuators positions,
      /// ranked list of policies and generating new policy
      /// \param actuators: vector list of robot's actuators
      /// \param sensors: vector list of robot's sensors
      /// \param t:
      /// \param step:
      void update(
              const std::vector< ActuatorPtr > &actuators,
              const std::vector< SensorPtr > &sensors,
              double t,
              double step) override;

      protected:
//      /**
//      * Request handler to modify the neural network
//      */
//      void modify(ConstModifyNeuralNetworkPtr &req);
//      boost::mutex networkMutex_; // Mutex for stepping / updating the network

      /// \brief Ranked list of used splines
      class PolicySave
      {
        public:
        PolicySave(
                double fitness,
                PolicyPtr &p)
                : policy_(p)
                , fitness_(fitness)
        {
        }

        bool operator>(const PolicySave &_ps) const
        {
          return this->fitness_ > _ps.fitness_;
        }

        PolicyPtr policy_;

        double fitness_;
      };

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

      /// \brief = 30.0; // evaluation time for each policy
      static const double EVALUATION_RATE;

      /// \brief = 0.8; // starting value for sigma
      static const double SIGMA_START_VALUE;

      /// \brief = 0.2;
      static const double SIGMA_TAU_CORRECTION;

      /// \brief = 5; // seconds
      static const double CYCLE_LENGTH;

      /// \brief = 0.98; // sigma decay
      static const double SIGMA_DECAY_SQUARED;

      template < typename ActuatorContainer, typename SensorContainer >
      void update(
              const ActuatorContainer &actuators,
              const SensorContainer &/*sensors*/,
              double t,
              double step)
      {
        //        boost::mutex::scoped_lock lock(networkMutex_);
        if (policyLoadPath_ == "")
        {
          if (start_eval_time_ < 0)
          {
            start_eval_time_ = t;
          }

          // Evaluate policy on certain time limit
          if ((t - start_eval_time_) > evaluation_rate_
              and generationCounter_ < maxEvaluations_)
          {
            this->updatePolicy();
            start_eval_time_ = t;
            evaluator_->start();
          }
        }

        // generate outputs
        double *output_vector = new double[numActuators_];
        this->generateOutput(t, output_vector);

        // Send new signals to the actuators
        size_t p = 0;
        for (auto actuator: actuators)
        {
          actuator->update(&output_vector[p], step);
          p += actuator->outputs();
        }

        delete[] output_vector;
      }

      struct Config
      {
        std::string algorithm_type;
        size_t evaluation_rate;
        size_t interpolation_spline_size;
        size_t max_evaluations;
        size_t max_ranked_policies;
        double noise_sigma;
        double sigma_tau_correction;
        size_t source_y_size;
        size_t update_step;
        std::string policy_load_path;
      };

      private:
//        /**
//         * Transport node
//         */
//        ::gazebo::transport::NodePtr node_;
//
//        /**
//         * Network modification subscriber
//         */
//        ::gazebo::transport::SubscriberPtr alterSub_;

      /// \brief Generate new policy
      void GenerateInitPolicy();


      /// \brief Generate cache policy
      void generateCache();

      /// \brief Evaluate the current policy and generate new
      void updatePolicy();


      /// \brief  Load saved policy from JSON file
      void LoadPolicy(std::string const &policy_path);


      /// \brief Generate interpolated spline based on number of sampled control
      /// points in 'source_y'
      /// \param source_y: set of control points over which interpolation is
      /// generated
      /// \param destination_y: set of interpolated control points (default 100)
      void InterpolateCubic(
              Policy *const source_y,
              Policy *destination_y);

      /// \brief Increment number of sampling points for policy
      void increaseSplinePoints();

      /// \brief Randomly select two policies and return the one with higher
      /// fitness
      /// \return an iterator from 'ranked_policies_' map
      std::map< double, RLPower::PolicyPtr >::iterator BinarySelection();

      /// \brief Extracts the value of the current_policy in x=time using linear
      /// interpolation
      /// Writes the output in output_vector
      void generateOutput(
              const double time,
              double *output_vector);

      /// \brief Retrieves fitness for the current policy
      /// \return
      double Fitness();

      /// \brief Writes current spline to file
      void LogCurrentSpline();

      /// \brief Writes best 10 splines to file
      void LogBestSplines();

      /// \brief Pointer to the current policy
      PolicyPtr current_policy_ = NULL;

      /// \brief Pointer to the interpolated current_policy_ (default 100)
      PolicyPtr interpolation_cache_ = NULL;

      /// \brief Pointer to the fitness evaluator
      EvaluatorPtr evaluator_ = NULL;

      /// \brief Number of current generation
      size_t generationCounter_;

      /// \brief Number of 'interpolation_cache_' sample points
      size_t numInterpolationPoints_;

      /// \brief Maximal number of stored ranked policies
      size_t maxRankedPolicies_;

      /// \brief Maximal number of evaluations
      size_t maxEvaluations_;

      /// \brief Number of actuators
      size_t numActuators_;

      /// \brief
      size_t source_y_size_;

      /// \brief
      size_t stepRate_;

      /// \brief Number of evaluations after which sampling size increases
      size_t updateStep_;


      double cycle_start_time_;


      double evaluation_rate_;

      /// \brief Noise in generatePolicy() function
      double sigma_;

      /// \brief Tau deviation for self-adaptive sigma
      double tau_;


      double start_eval_time_;

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

#endif  //  REVOLVEBRAIN_BRAIN_RLPOWER_H_
