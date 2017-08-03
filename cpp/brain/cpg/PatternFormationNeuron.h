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
* Date: November 8, 2016
*
*/

#ifndef REVOLVE_BRAIN_PATTERNFORMATIONNEURON_H
#define REVOLVE_BRAIN_PATTERNFORMATIONNEURON_H

#include "CPGNeuron.h"
#include "../Sensor.h"

namespace revolve
{
  namespace brain
  {
    namespace cpg
    {

      class PatternFormationNeuron
              : public revolve::brain::cpg::Neuron
      {
        public:
        /// \brief PatternFormationNeuron constructor
        /// \param alpha parameter TODO limits
        /// \param theta parameter TODO limits
        /// \param weights list of weights. The dimensions should correspond to
        /// the size of inputs
        /// passed in the update function (dimension: number of inputs)
        PatternFormationNeuron(real_t alpha,
                               real_t theta,
                               std::vector<real_t> weights);

        /// \brief PatternFormationNeuron constructor. Alpha to default 1. Theta
        /// to default 0.
        /// \param weights list of weights. The dimensions should correspond to
        /// the size of inputs passed in the update function
        /// (dimension: number of inputs)
        PatternFormationNeuron(std::vector<real_t> weights)
                : PatternFormationNeuron(1, 0, weights)
        {}

        virtual ~PatternFormationNeuron();

        /// \brief Update the pattern formation neuron to the step (t+1)
        /// \param inputs std::vector<real> getting sensor and RGE inputs.
        /// But really, anything you want. MUST be the same size as the weights
        /// in the constructor, ot will throw an
        /// revolve::brain::cpg::Neuron::invalid_input_exception exception.
        /// \param delta_time time passed since the last update. Value in
        /// seconds expected to be positive and relativly small. Big time steps
        /// could have weird effects on the network.
        /// \return std::vector<real> of size 1.
        /// \throws invalid_input_exception if input vector is not of the
        /// correct size (dimension of internal weights)
        virtual std::vector<real_t> update(std::vector<real_t> inputs,
                                           real_t delta_time) override;

        protected:
        /// \brief Generating the weighted average of all the inputs.
        /// \param inputs vector of inputs. Size has to be the same as weights
        /// \return weighted average of all inputs
        real_t generateInput(std::vector<real_t> inputs) const;

        /// \brief Pattern formation from the combined inputs
        /// \param combined_inputs (output of
        /// PatternFormationNeuron::generateInput)
        /// \return final neuron output
        real_t output(real_t combined_inputs) const;

        public:
        /// \brief
        real_t getAlpha() const;

        /// \brief
        void setAlpha(real_t alpha);

        /// \brief value from 0 to 1, does not throw exceptions if value is
        /// outside the allowed domain
        real_t setAlphaPercentage(real_t alpha);

        /// \brief
        real_t calculateAlphaPercentage(real_t alpha) const;

        /// \brief
        real_t calculateAlphaFromPercentage(real_t alpha) const;

        /// \brief
        real_t getTheta() const;

        /// \brief
        void setTheta(real_t theta);

        /// \brief value from 0 to 1, does not throw exceptions if value is
        /// outside the allowed domain
        real_t setThetaPercentage(real_t theta);

        /// \brief
        real_t calculateThetaPercentage(real_t theta) const;

        /// \brief
        real_t calculateThetaFromPercentage(real_t theta) const;

        /// \brief
        const std::vector<real_t> &getWeights() const;

        /// \brief Is normalizing the weights
        /// \param weights new weights
        void setWeights(const std::vector<real_t> &weights);

        public:
        /// \brief
        const real_t ALPHA_MIN = 0;

        /// \brief
        const real_t ALPHA_MAX = 10;

        /// \brief
        const real_t THETA_MIN = -5;

        /// \brief
        const real_t THETA_MAX = 5;

        protected:
        /// \brief
        real_t alpha;

        /// \brief
        real_t theta;

        /// \brief
        std::vector<real_t> weights;
      };
    }
  }
}

#endif  // REVOLVE_BRAIN_PATTERNFORMATIONNEURON_H
