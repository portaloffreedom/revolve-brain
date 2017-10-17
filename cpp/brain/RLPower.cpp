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

#include <fstream>
#include <map>
#include <random>

#include <gsl/gsl_spline.h>

#include <yaml-cpp/yaml.h>

#include "RLPower.h"

using namespace revolve::brain;

RLPower::RLPower(
        std::string modelName,
        Config brain,
        EvaluatorPtr evaluator,
        size_t n_actuators
)
        : evaluator_(evaluator)
        , generationCounter_(0)
        , numInterpolationPoints_(brain.interpolation_spline_size)
        , maxRankedPolicies_(brain.max_ranked_policies)
        , maxEvaluations_(brain.max_evaluations)
        , numActuators_(n_actuators)
        , source_y_size_(brain.source_y_size)
        , updateStep_(brain.update_step)
        , cycle_start_time_(-1)
        , evaluation_rate_(brain.evaluation_rate)
        , sigma_(brain.noise_sigma)
        , tau_(brain.sigma_tau_correction)
        , start_eval_time_(-1)
        , robotName_(modelName)
        , algorithmType_(brain.algorithm_type)
        , policyLoadPath_(brain.policy_load_path)
{
  // // Create transport node
  // node_.reset(new ::gazebo::transport::Node());
  // node_->Init();
  //
  // // Listen to network modification requests
  // alterSub_ = node_->Subscribe("~/" + modelName + "/modify_neural_network",
  //                              &RLPower::modify, this);

  // Read out brain configuration attributes
  std::cout << std::endl << "Initialising RLPower, type "
            << this->algorithmType_ << std::endl << std::endl;

  this->stepRate_ = this->numInterpolationPoints_ / this->source_y_size_;

  if (this->policyLoadPath_ == "")
  {
    // Generate first random policy
    this->GenerateInitPolicy();
  }
  else
  {
    this->LoadPolicy(this->policyLoadPath_);
  }

  // Start the evaluator
  this->evaluator_->start();
}

RLPower::~RLPower()
{
  // `boost::shared_ptr< Policy >` should take care of memory management for us
}

void RLPower::update(
        const std::vector< ActuatorPtr > &actuators,
        const std::vector< SensorPtr > &sensors,
        double t,
        double step)
{
  this->update< std::vector< ActuatorPtr >,
                std::vector< SensorPtr > >(
          actuators,
          sensors,
          t,
          step);
}

void RLPower::GenerateInitPolicy()
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::normal_distribution< double > dist(0, this->sigma_);

  // Init first random controller
  if (not this->current_policy_)
  {
    this->current_policy_ = std::make_shared< Policy >(this->numActuators_);
  }

  for (size_t i = 0; i < this->numActuators_; i++)
  {
    Spline spline(this->source_y_size_);
    for (size_t j = 0; j < this->source_y_size_; j++)
    {
      spline[j] = dist(mt);
    }
    this->current_policy_->at(i) = spline;
  }

  // Init of empty cache
  if (not this->interpolation_cache_)
  {
    this->interpolation_cache_ =
            std::make_shared< Policy >(this->numActuators_);
  }

  for (size_t i = 0; i < this->numActuators_; i++)
  {
    this->interpolation_cache_->at(i).resize(this->numInterpolationPoints_, 0);
  }

  this->generateCache();
}

void RLPower::LoadPolicy(std::string const &policy_path)
{
  YAML::Node policy_file = YAML::LoadFile(policy_path);
  if (policy_file.IsNull())
  {
    std::cout << "Failed to load the policy file." << std::endl;
    return;
  }

  // Init first random controller
  if (not this->current_policy_)
  {
    this->current_policy_ = std::make_shared< Policy >(this->numActuators_);
  }

  std::cout << "evaluation: " << policy_file[0]["evaluation"] << std::endl;
  std::cout << "steps: " << policy_file[0]["steps"] << std::endl;
  std::cout << "velocity: " << policy_file[0]["population"][0]["velocity"]
            << std::endl;

  size_t k = 0;
  this->source_y_size_ = policy_file[0]["steps"].as< uint >();
  YAML::Node policy = policy_file[0]["population"][0]["policy"];

  if (this->source_y_size_ * this->numActuators_ not_eq policy.size())
  {
    std::cout << "Number of (n_spline_points) is not equal to "
            "(n_actuators * n_steps)!" << std::endl;
    return;
  }

  for (size_t i = 0; i < this->numActuators_; i++)
  {
    Spline spline(this->source_y_size_);
    for (size_t j = 0; j < this->source_y_size_; j++)
    {
      spline[j] = policy[k++].as< double >();
    }
    this->current_policy_->at(i) = spline;
  }

  // Init of empty cache
  if (not this->interpolation_cache_)
  {
    this->interpolation_cache_ = std::make_shared< Policy >(numActuators_);
  }

  for (size_t i = 0; i < this->numActuators_; i++)
  {
    this->interpolation_cache_->at(i).resize(this->numInterpolationPoints_, 0);
  }

  this->generateCache();
}

void RLPower::generateCache()
{
  this->InterpolateCubic(this->current_policy_.get(),
                         this->interpolation_cache_.get());
}

void RLPower::updatePolicy()
{
  // Calculate fitness for current policy
  double curr_fitness = this->Fitness();

  // Insert ranked policy in list
  PolicyPtr policy_copy = std::make_shared< Policy >(this->numActuators_);
  for (size_t i = 0; i < this->numActuators_; i++)
  {
    Spline &spline = this->current_policy_->at(i);
    policy_copy->at(i) = Spline(spline.begin(), spline.end());

    spline.resize(this->source_y_size_);
  }
  this->rankedPolicies_.insert({curr_fitness, policy_copy});

  // Remove worst policies
  while (this->rankedPolicies_.size() > maxRankedPolicies_)
  {
    auto last = std::prev(this->rankedPolicies_.end());
    this->rankedPolicies_.erase(last);
  }

  // Print-out current status to the terminal
  std::cout << this->robotName_ << ":"
            << this->generationCounter_ << " rankedPolicies_:";
  for (auto const &it : this->rankedPolicies_)
  {
    double fitness = it.first;
    std::cout << " " << fitness;
  }
  std::cout << std::endl;

  // Write fitness and genomes log to output files
  this->LogCurrentSpline();
  this->LogBestSplines();

  // Update generation counter and check is it finished
  this->generationCounter_++;
  if (this->generationCounter_ == this->maxEvaluations_)
  {
    std::exit(0);
  }

  // Increase spline points if it is a time
  if (this->generationCounter_ % this->updateStep_ == 0)
  {
    this->increaseSplinePoints();
  }

  /// Actual policy generation

  /// Determine which mutation operator to use
  /// Default, for algorithms A and B, is used standard normal distribution with
  /// decaying sigma
  /// For algorithms C and D, is used normal distribution with self-adaptive
  /// sigma
  std::random_device rd;
  std::mt19937 mt(rd());

  if (this->algorithmType_ == "C" or this->algorithmType_ == "D")
  {
    // uncorrelated mutation with one step size
    std::mt19937 sigma_mt(rd());
    std::normal_distribution< double > sigma_dist(0, 1);
    this->sigma_ *= std::exp(this->tau_ * sigma_dist(sigma_mt));
  }
  else
  {
    // Default is decaying sigma
    if (this->rankedPolicies_.size() >= this->maxRankedPolicies_)
    {
      this->sigma_ *= this->SIGMA_DECAY_SQUARED;
    }
  }
  std::normal_distribution< double > dist(0, sigma_);

  /// Determine which selection operator to use
  /// Default, for algorithms A and C, is used ten parent crossover
  /// For algorithms B and D, is used two parent crossover with binary
  /// tournament selection
  if (this->rankedPolicies_.size() < this->maxRankedPolicies_)
  {
    // Generate random policy if number of stored policies is less then
    // 'maxRankedPolicies_'
    for (size_t i = 0; i < this->numActuators_; i++)
    {
      for (size_t j = 0; j < this->source_y_size_; j++)
      {
        (*this->current_policy_)[i][j] = dist(mt);
      }
    }
  }
  else
  {
    // Generate new policy using weighted crossover operator
    double total_fitness = 0;
    if (this->algorithmType_ == "B" or this->algorithmType_ == "D")
    {
      // k-selection tournament
      auto parent1 = this->BinarySelection();
      auto parent2 = parent1;
      while (parent2 == parent1)
      {
        parent2 = this->BinarySelection();
      }

      double fitness1 = parent1->first;
      double fitness2 = parent2->first;

      PolicyPtr policy1 = parent1->second;
      PolicyPtr policy2 = parent2->second;

      // TODO: Verify what should be total fitness in binary
      total_fitness = fitness1 + fitness2;

      // For each spline
      for (size_t i = 0; i < this->numActuators_; i++)
      {
        // And for each control point
        for (size_t j = 0; j < this->source_y_size_; j++)
        {
          // Apply modifier
          double spline_point = 0;
          spline_point += ((policy1->at(i)[j] - (*this->current_policy_)[i][j]))
                          * (fitness1 / total_fitness);
          spline_point += ((policy2->at(i)[j] - (*this->current_policy_)[i][j]))
                          * (fitness2 / total_fitness);

          // Add a mutation + current
          // TODO: Verify do we use current in this case
          spline_point += dist(mt) + (*this->current_policy_)[i][j];

          // Set a newly generated point as current
          (*this->current_policy_)[i][j] = spline_point;
        }
      }
    }
    else
    {
      // Default is all parents selection

      // Calculate first total sum of fitnesses
      for (auto const &it : this->rankedPolicies_)
      {
        double fitness = it.first;
        total_fitness += fitness;
      }

      // For each spline
      // TODO: Verify that this should is correct formula
      for (size_t i = 0; i < this->numActuators_; i++)
      {
        // And for each control point
        for (size_t j = 0; j < this->source_y_size_; j++)
        {
          // Apply modifier
          double spline_point = 0;
          for (auto const &it : this->rankedPolicies_)
          {
            double fitness = it.first;
            PolicyPtr policy = it.second;

            spline_point +=
                    ((policy->at(i)[j] - (*this->current_policy_)[i][j]))
                    * (fitness / total_fitness);
          }

          // Add a mutation + current
          // TODO: Verify do we use 'currentPolicy_' in this case
          spline_point += dist(mt) + (*this->current_policy_)[i][j];

          // Set a newly generated point as current
          (*this->current_policy_)[i][j] = spline_point;
        }
      }
    }
  }

  // cache update
  this->generateCache();
}

void RLPower::InterpolateCubic(
        Policy *const source_y,
        Policy *destination_y)
{
  const size_t source_y_size_ = (*source_y)[0].size();
  const size_t destination_y_size = (*destination_y)[0].size();

  const size_t N = source_y_size_ + 1;
  auto *x = new double[N];
  auto *y = new double[N];
  auto *x_new = new double[destination_y_size];

  gsl_interp_accel *acc = gsl_interp_accel_alloc();
  const gsl_interp_type *t = gsl_interp_cspline_periodic;
  gsl_spline *spline = gsl_spline_alloc(t, N);

  // init x
  double step_size = CYCLE_LENGTH / source_y_size_;
  for (size_t i = 0; i < N; i++)
  {
    x[i] = step_size * i;
  }

  // init x_new
  step_size = CYCLE_LENGTH / destination_y_size;
  for (size_t i = 0; i < destination_y_size; i++)
  {
    x_new[i] = step_size * i;
  }

  for (size_t j = 0; j < this->numActuators_; j++)
  {
    Spline &source_y_line = source_y->at(j);
    Spline &destination_y_line = destination_y->at(j);

    // init y
    // TODO use memcpy
    for (size_t i = 0; i < source_y_size_; i++)
    {
      y[i] = source_y_line[i];
    }

    // make last equal to first
    y[N - 1] = y[0];

    gsl_spline_init(spline, x, y, N);

    for (size_t i = 0; i < destination_y_size; i++)
    {
      destination_y_line[i] = gsl_spline_eval(spline, x_new[i], acc);
    }
  }

  gsl_spline_free(spline);
  gsl_interp_accel_free(acc);

  delete[] x_new;
  delete[] y;
  delete[] x;
}

void RLPower::increaseSplinePoints()
{
  this->source_y_size_++;

  // LOG code
  this->stepRate_ = this->numInterpolationPoints_ / this->source_y_size_;
  std::cout << "New samplingSize_=" << this->source_y_size_
            << ", and stepRate_=" << this->stepRate_ << std::endl;

  // Copy current policy for resizing
  Policy policy_copy(this->current_policy_->size());
  for (size_t i = 0; i < this->numActuators_; i++)
  {
    Spline &spline = this->current_policy_->at(i);
    policy_copy[i] = Spline(spline.begin(), spline.end());

    spline.resize(this->source_y_size_);
  }

  this->InterpolateCubic(&policy_copy, this->current_policy_.get());

  // for every ranked policy
  for (auto &it : this->rankedPolicies_)
  {
    PolicyPtr policy = it.second;

    for (size_t j = 0; j < this->numActuators_; j++)
    {
      Spline &spline = policy->at(j);
      policy_copy[j] = Spline(spline.begin(), spline.end());
      spline.resize(this->source_y_size_);
    }
    this->InterpolateCubic(&policy_copy, policy.get());
  }
}

std::map< double, RLPower::PolicyPtr >::iterator RLPower::BinarySelection()
{
  std::random_device rd;
  std::mt19937 umt(rd());
  std::uniform_int_distribution< size_t >
          udist(0, this->maxRankedPolicies_ - 1);

  // Select two different numbers from uniform distribution
  // U(0, maxRankedPolicies_ - 1)
  size_t pindex1, pindex2;
  pindex1 = udist(umt);
  do
  {
    pindex2 = udist(umt);
  } while (pindex1 == pindex2);

  // Set iterators to begin of the 'rankedPolicies_' map
  auto individual1 = this->rankedPolicies_.begin();
  auto individual2 = this->rankedPolicies_.begin();

  // Move iterators to indices positions
  std::advance(individual1, pindex1);
  std::advance(individual2, pindex2);

  double fitness1 = individual1->first;
  double fitness2 = individual2->first;

  return fitness1 > fitness2 ? individual1 : individual2;
}

void RLPower::generateOutput(
        const double time,
        double *output_vector)
{
  if (this->cycle_start_time_ < 0)
  {
    this->cycle_start_time_ = time;
  }

  // get correct X value (between 0 and CYCLE_LENGTH)
  double x = time - this->cycle_start_time_;
  while (x >= RLPower::CYCLE_LENGTH)
  {
    this->cycle_start_time_ += RLPower::CYCLE_LENGTH;
    x = time - this->cycle_start_time_;
  }

  // adjust X on the cache coordinate space
  x = (x / CYCLE_LENGTH) * this->numInterpolationPoints_;
  // generate previous and next values
  int x_a = (static_cast<int>(x)) % this->numInterpolationPoints_;
  int x_b = (x_a + 1) % this->numInterpolationPoints_;

  // linear interpolation for every actuator
  for (size_t i = 0; i < this->numActuators_; i++)
  {
    double y_a = this->interpolation_cache_->at(i)[x_a];
    double y_b = this->interpolation_cache_->at(i)[x_b];

    output_vector[i] = y_a + ((y_b - y_a) * (x - x_a) / (x_b - x_a));
  }
}

double RLPower::Fitness()
{
  // Calculate fitness for current policy
  return this->evaluator_->fitness();
}

void RLPower::LogCurrentSpline()
{
  std::ofstream outputFile;
  outputFile.open(this->robotName_ + ".log",
                  std::ios::app | std::ios::out | std::ios::ate);
  outputFile << "- generation: " << this->generationCounter_ << std::endl;
  outputFile << "  velocities:" << std::endl;
  for (auto const &it : this->rankedPolicies_)
  {
    double fitness = it.first;
    outputFile << "  - " << fitness << std::endl;
  }
  outputFile.close();
}

void RLPower::LogBestSplines()
{
  std::ofstream outputFile;
  outputFile.open(this->robotName_ + ".policy",
                  std::ios::app | std::ios::out | std::ios::ate);
  outputFile << "- evaluation: " << generationCounter_ << std::endl;
  outputFile << "  steps: " << source_y_size_ << std::endl;
  outputFile << "  population:" << std::endl;
  for (auto const &it : this->rankedPolicies_)
  {
    double fitness = it.first;
    PolicyPtr policy = it.second;
    outputFile << "   - velocity: " << fitness << std::endl;
    outputFile << "     policy:" << std::endl;
    for (size_t i = 0; i < policy->size(); i++)
    {
      Spline &spline = policy->at(i);
      for (size_t j = 0; j < spline.size(); j++)
      {
        outputFile << "      - " << spline.at(j) << std::endl;
      }
    }
  }
  outputFile.close();
}

/// \brief max number of evaluations
const size_t RLPower::MAX_EVALUATIONS = 1000;

/// \brief max length of policies vector
const size_t RLPower::MAX_RANKED_POLICIES = 10;

/// \brief number of data points for the interpolation cache
const size_t RLPower::INTERPOLATION_CACHE_SIZE = 100;

/// \brief number of initially sampled spline points
const size_t RLPower::INITIAL_SPLINE_SIZE = 3;

/// \brief after # generations, it increases the number of spline points
const size_t RLPower::UPDATE_STEP = 100;

/// \brief evaluation time for each policy
const double RLPower::EVALUATION_RATE = 30.0;

/// \brief starting value for sigma
const double RLPower::SIGMA_START_VALUE = 0.8;

/// \brief
const double RLPower::SIGMA_TAU_CORRECTION = 0.2;

/// \brief seconds
const double RLPower::CYCLE_LENGTH = 5;

/// \brief sigma decay
const double RLPower::SIGMA_DECAY_SQUARED = 0.98;
