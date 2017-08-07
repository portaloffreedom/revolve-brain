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
* Author:  Milan Jelisavcic
* Date: March 28, 2016
*
*/

#include <map>
#include <fstream>
#include <string>
#include <random>

#include <gsl/gsl_spline.h>
#include <yaml-cpp/yaml.h>

#include "RLPowerLearner.h"

using namespace revolve::brain;

RLPowerLearner::RLPowerLearner(std::string &_modelName,
                               Config _brain,
                               size_t _numWeightVectors)
        : numInterpolationPoints_(_brain.interpolation_spline_size)
          , generationCounter_(0)
          , maxRankedPolicies_(_brain.max_ranked_policies)
          , maxEvaluations_(_brain.max_evaluations)
          , numActuators_(_numWeightVectors)
          , numSteps_(_brain.source_y_size)
          , updateStep_(_brain.update_step)
          , sigma_(_brain.noise_sigma)
          , tau_(_brain.sigma_tau_correction)
          , robotName_(_modelName)
          , algorithmType_(_brain.algorithm_type)
          , policyLoadPath_(_brain.policy_load_path)
{
  // Read out brain configuration attributes
  std::cout << std::endl << "Initialising RLPowerLearner, type "
            << algorithmType_ << std::endl << std::endl;


  stepRate_ = numInterpolationPoints_ / numSteps_;

  if (policyLoadPath_ == "")
  {
    // Generate first random policy
    this->GenerateInitPolicy();
  }
  else
  {
    this->LoadPolicy(policyLoadPath_);
  }
}

RLPowerLearner::~RLPowerLearner()
{
  // `boost::shared_ptr< Policy >` should take care of memory management for us
}


void RLPowerLearner::GenerateInitPolicy()
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::normal_distribution<double> dist(0, this->sigma_);

  // Init first random controller
  if (not currentPolicy_)
  {
    currentPolicy_ = std::make_shared<Policy>(numActuators_);
  }

  for (size_t i = 0; i < numActuators_; i++)
  {
    Spline spline(numSteps_);
    for (size_t j = 0; j < numSteps_; j++)
    {
      spline[j] = dist(mt);
    }
    currentPolicy_->at(i) = spline;
  }
}

void RLPowerLearner::LoadPolicy(const std::string &_policyPath)
{
  YAML::Node policy_file = YAML::LoadFile(_policyPath);
  if (policy_file.IsNull())
  {
    std::cout << "Failed to load the policy file." << std::endl;
    return;
  }

  // Init first random controller
  if (not this->currentPolicy_)
  {
    this->currentPolicy_ = std::make_shared<Policy>(this->numActuators_);
  }

  std::cout << "evaluation: " << policy_file[0]["evaluation"] << std::endl;
  std::cout << "steps: " << policy_file[0]["steps"] << std::endl;
  std::cout << "velocity: " << policy_file[0]["population"][0]["velocity"]
            << std::endl;

  size_t k = 0;
  this->numSteps_ = policy_file[0]["steps"].as<uint>();
  YAML::Node policy = policy_file[0]["population"][0]["policy"];

  if (this->numSteps_ * this->numActuators_ != policy.size())
  {
    std::cout << "Number of (n_spline_points) is not equal to "
            "(n_actuators * n_steps)!" << std::endl;
    return;
  }

  for (size_t i = 0; i < this->numActuators_; i++)
  {
    Spline spline(this->numSteps_);
    for (size_t j = 0; j < this->numSteps_; j++)
    {
      spline[j] = policy[k++].as<double>();
    }
    this->currentPolicy_->at(i) = spline;
  }
}

void RLPowerLearner::reportFitness(std::string id,
                                   PolicyPtr genotype,
                                   double curr_fitness)
{
  // Insert ranked policy in list
  PolicyPtr policy_copy = std::make_shared<Policy>(numActuators_);
  for (size_t i = 0; i < numActuators_; i++)
  {
    Spline &spline = currentPolicy_->at(i);
    policy_copy->at(i) = Spline(spline.begin(), spline.end());

    spline.resize(numSteps_);
  }
  rankedPolicies_.insert({curr_fitness, policy_copy});

  // Remove worst policies
  while (rankedPolicies_.size() > maxRankedPolicies_)
  {
    auto last = std::prev(rankedPolicies_.end());
    rankedPolicies_.erase(last);
  }

  // Print-out current status to the terminal
  std::cout << robotName_ << ":"
            << generationCounter_ << " rankedPolicies_:";
  for (auto const &it : rankedPolicies_)
  {
    double fitness = it.first;
    std::cout << " " << fitness;
  }
  std::cout << std::endl;

  // Write fitness and genomes log to output files
  //    this->LogCurrentSpline();
  this->LogBestSplines();

  // Update generation counter and check is it finished
  generationCounter_++;
  if (generationCounter_ == maxEvaluations_)
  {
    std::exit(0);
  }

  // Increase spline points if it is a time
  if (updateStep_ > 0 && generationCounter_ % updateStep_ == 0)
  {
    this->IncreaseSplinePoints();
  }

  /// Actual policy generation

  /// Determine which mutation operator to use
  /// Default, for algorithms A and B, is used standard normal
  /// distribution with decaying sigma
  /// For algorithms C and D, is used normal distribution with
  /// self-adaptive sigma
  std::random_device rd;
  std::mt19937 mt(rd());

  if (algorithmType_ == "C" || algorithmType_ == "D")
  {
    // uncorrelated mutation with one step size
    std::mt19937 sigma_mt(rd());
    std::normal_distribution<double> sigma_dist(0,
                                                1);
    sigma_ = sigma_
                   * std::exp(tau_ * sigma_dist(sigma_mt));
  }
  else
  {
    // Default is decaying sigma
    if (rankedPolicies_.size() >= maxRankedPolicies_)
    {
      sigma_ *= SIGMA_DECAY_SQUARED;
    }
  }
  std::normal_distribution<double> dist(0, sigma_);

  /// Determine which selection operator to use
  /// Default, for algorithms A and C, is used
  /// ten parent crossover
  /// For algorithms B and D, is used two parent crossover
  /// with binary tournament selection
  if (rankedPolicies_.size() < maxRankedPolicies_)
  {
    // Generate random policy if number of stored policies
    // is less then 'maxRankedPolicies_'
    for (size_t i = 0; i < numActuators_; i++)
    {
      for (size_t j = 0; j < numSteps_; j++)
      {
        (*currentPolicy_)[i][j] = dist(mt);
      }
    }
  }
  else
  {
    // Generate new policy using weighted crossover operator
    double total_fitness = 0;
    if (algorithmType_ == "B" || algorithmType_ == "D")
    {
      // k-selection tournament
      auto parent1 = BinarySelection();
      auto parent2 = parent1;
      while (parent2 == parent1)
      {
        parent2 = BinarySelection();
      }

      double fitness1 = parent1->first;
      double fitness2 = parent2->first;

      PolicyPtr policy1 = parent1->second;
      PolicyPtr policy2 = parent2->second;

      // TODO: Verify what should be total fitness in binary
      total_fitness = fitness1 + fitness2;

      // For each spline
      for (size_t i = 0; i < numActuators_; i++)
      {
        // And for each control point
        for (size_t j = 0; j < numSteps_; j++)
        {
          // Apply modifier
          double spline_point = 0;
          spline_point += ((policy1->at(i)[j] - (*currentPolicy_)[i][j]))
                          * (fitness1 / total_fitness);
          spline_point += ((policy2->at(i)[j] - (*currentPolicy_)[i][j]))
                          * (fitness2 / total_fitness);

          // Add a mutation + current
          // TODO: Verify do we use current in this case
          spline_point += dist(mt) + (*currentPolicy_)[i][j];

          // Set a newly generated point as current
          (*currentPolicy_)[i][j] = spline_point;
        }
      }
    }
    else
    {
      // Default is all parents selection

      // Calculate first total sum of fitnesses
      for (auto const &it : rankedPolicies_)
      {
        double fitness = it.first;
        total_fitness += fitness;
      }

      // For each spline
      // TODO: Verify that this should is correct formula
      for (size_t i = 0; i < numActuators_; i++)
      {
        // And for each control point
        for (size_t j = 0; j < numSteps_; j++)
        {
          // Apply modifier
          double spline_point = 0;
          for (auto const &it : rankedPolicies_)
          {
            double fitness = it.first;
            PolicyPtr policy = it.second;

            spline_point += ((policy->at(i)[j] - (*currentPolicy_)[i][j]))
                            * (fitness / total_fitness);
          }

          // Add a mutation + current
          // TODO: Verify do we use 'currentPolicy_' in this case
          spline_point += dist(mt) + (*currentPolicy_)[i][j];

          // Set a newly generated point as current
          (*currentPolicy_)[i][j] = spline_point;
        }
      }
    }
  }
}

PolicyPtr RLPowerLearner::currentGenotype()
{
  return currentPolicy_;
}

void RLPowerLearner::InterpolateCubic(Policy *const _sourceY,
                                      Policy *_destinationY)
{
  const size_t source_y_size_ = (*_sourceY)[0].size();
  const size_t destination_y_size = (*_destinationY)[0].size();

  const size_t N = source_y_size_ + 1;
  double *x = new double[N];
  double *y = new double[N];
  double *x_new = new double[destination_y_size];

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

  for (size_t j = 0; j < numActuators_; j++)
  {
    Spline &source_y_line = _sourceY->at(j);
    Spline &destination_y_line = _destinationY->at(j);

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

void RLPowerLearner::IncreaseSplinePoints()
{
  numSteps_++;

  // LOG code
  stepRate_ = numInterpolationPoints_ / numSteps_;
  std::cout << "New samplingSize_=" << numSteps_
            << ", and stepRate_=" << stepRate_ << std::endl;

  // Copy current policy for resizing
  Policy policy_copy(currentPolicy_->size());
  for (size_t i = 0; i < numActuators_; i++)
  {
    Spline &spline = currentPolicy_->at(i);
    policy_copy[i] = Spline(spline.begin(), spline.end());

    spline.resize(numSteps_);
  }

  this->InterpolateCubic(&policy_copy, currentPolicy_.get());

  // for every ranked policy
  for (auto &it : rankedPolicies_)
  {
    PolicyPtr policy = it.second;

    for (size_t j = 0; j < numActuators_; j++)
    {
      Spline &spline = policy->at(j);
      policy_copy[j] = Spline(spline.begin(), spline.end());
      spline.resize(numSteps_);
    }
    this->InterpolateCubic(&policy_copy, policy.get());
  }
}

std::map<double, PolicyPtr>::iterator RLPowerLearner::BinarySelection()
{
  std::random_device rd;
  std::mt19937 umt(rd());
  std::uniform_int_distribution<size_t> udist(0, maxRankedPolicies_ - 1);

  // Select two different numbers from uniform distribution
  // U(0, maxRankedPolicies_ - 1)
  size_t pindex1, pindex2;
  pindex1 = udist(umt);
  do
  {
    pindex2 = udist(umt);
  } while (pindex1 == pindex2);

  // Set iterators to begin of the 'rankedPolicies_' map
  auto individual1 = rankedPolicies_.begin();
  auto individual2 = rankedPolicies_.begin();

  // Move iterators to indices positions
  std::advance(individual1, pindex1);
  std::advance(individual2, pindex2);

  double fitness1 = individual1->first;
  double fitness2 = individual2->first;

  return fitness1 > fitness2 ? individual1 : individual2;
}

void RLPowerLearner::LogCurrentSpline()
{
  std::ofstream outputFile;
  outputFile.open(robotName_ + ".log",
                  std::ios::app | std::ios::out | std::ios::ate);
  outputFile << "- generation: " << generationCounter_ << std::endl;
  outputFile << "  velocities:" << std::endl;
  for (auto const &it : rankedPolicies_)
  {
    double fitness = it.first;
    outputFile << "  - " << fitness << std::endl;
  }
  outputFile.close();
}

void RLPowerLearner::LogBestSplines()
{
  std::ofstream outputFile;
  outputFile.open(robotName_ + ".policy",
                  std::ios::app | std::ios::out | std::ios::ate);
  outputFile << "- evaluation: " << generationCounter_ << std::endl;
  outputFile << "  steps: " << numSteps_ << std::endl;
  outputFile << "  population:" << std::endl;
  for (auto const &it : rankedPolicies_)
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
const size_t RLPowerLearner::MAX_EVALUATIONS = 1000;

/// \brief max length of policies vector
const size_t RLPowerLearner::MAX_RANKED_POLICIES = 10;

/// \brief number of data points for the interpolation cache
const size_t RLPowerLearner::INTERPOLATION_CACHE_SIZE = 100;

/// \brief number of initially sampled spline points
const size_t RLPowerLearner::INITIAL_SPLINE_SIZE = 3;

/// \brief after # generations, it increases the number of spline points
const size_t RLPowerLearner::UPDATE_STEP = 100;

/// \brief evaluation time for each policy
const double RLPowerLearner::EVALUATION_RATE = 30.0;

/// \brief starting value for sigma
const double RLPowerLearner::SIGMA_START_VALUE = 0.8;

/// \brief
const double RLPowerLearner::SIGMA_TAU_CORRECTION = 0.2;

/// \brief seconds
const double RLPowerLearner::CYCLE_LENGTH = 5;

/// \brief sigma decay
const double RLPowerLearner::SIGMA_DECAY_SQUARED = 0.98;
