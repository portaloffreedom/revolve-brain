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
* Date November 1, 2016
*
*/

#ifndef REVOLVE_BRAIN_RYTHMGENERATIONNEURON_H
#define REVOLVE_BRAIN_RYTHMGENERATIONNEURON_H

#include <vector>

#include "CPGNeuron.h"

namespace revolve
{
  namespace brain
  {
    namespace cpg
    {
      class RythmGenerationNeuron
              : public revolve::brain::cpg::Neuron
      {
        public:
        /// \brief RythmGenerationNeuron constructor. Starting value of phi is 1
        /// \param weight the weight that determines the shape [0, 4.5]
        /// \param weight_neigbours the weights for the connected neighbours.
        /// This size must be `inputs.size()-1` in the update function
        /// \param c the phase difference [-2, 2]
        /// \param amplitude the amplitude determines influence of a
        /// flexor/extensor on the final output signal [−10, 10]
        /// \param offset the offset dims shape of the final output signal to
        /// the starting position [-0.1, 0.1]
        /// \throws invalid_parameter if one of the given parameters is out of
        /// range
        RythmGenerationNeuron(real_t weight,
                              std::vector<real_t> weight_neigbours,
                              real_t c,
                              real_t amplitude,
                              real_t offset);

        /// \brief
        virtual ~RythmGenerationNeuron();

        /// \brief  Update the rythm generator to the step (t+1)
        /// \param inputs std::vector<real_t> of size
        /// 1 + weight_neigbours.size() contaning the phi of the coupled
        /// RythmGenerationNeuron
        /// \param delta_time time passed since the last update. Value in
        /// seconds expected to be positive and relativly small. Big time steps
        /// could have weird effects on the network.
        /// \return std::vector<real_t> of size 2. First element has the output
        /// of the RythmGenerationNeuron, second element has the update phi
        /// value
        /// \throws invalid_input_exception if input vector is not of the
        /// correct size
        virtual std::vector<real_t> update(std::vector<real_t> inputs,
                                           real_t delta_time) override;

        protected:

        /// \brief Calculates the next phi value and returns it. It is NOT
        /// updating this->phi.
        /// \param inputs the phi of the coupled RythmGenerationNeuron
        /// (in position 0) and the phis of the neighbours
        /// nodes
        /// \param delta_time time passed since the last update. Value in
        /// seconds expected to be
        /// positive and relativly small. Big time steps could have weird
        /// effects on the network.
        /// \return new phi value
        real_t nextPhi(const std::vector<real_t> &inputs,
                       real_t delta_time) const;

        /// \brief  Calculates the output value of the neuron using the current
        /// phi value
        /// \return the output value of the RythmGenerationNeuron
        real_t output() const;

// GETTERS AND SETTERS
        public:
        /// \brief  Returns current phi value
        /// \return current phi value
        real_t Phi() const;

        /// \brief
        real_t Weight() const;

        /// \brief the weight that determines the shape [0, 1.0]
        void setWeight(real_t weight);

        /// \brief value from 0 to 1, does not throw exceptions if value is
        /// outside the allowed domain
        real_t setWeightPercentage(real_t weight);

        /// \brief
        real_t calculateWeightPercentage(real_t weight) const;

        /// \brief
        real_t calculateWeightFromPercentage(real_t weight) const;

        /// \brief
        real_t WeightNeighbour(size_t index) const;

        /// \brief the weight that determines the shape [0, 1.0]
        void setWeightNeighbour(real_t weight,
                                size_t index);

        /// \brief value from 0 to 1, does not throw exceptions if value is
        /// outside the allowed domain
        real_t setWeightNeighbourPercentage(real_t weight,
                                            size_t index);

        /// \brief
        real_t calculateWeightNeighbourPercentage(real_t weight,
                                                  size_t index) const;

        /// \brief
        real_t calculateWeightNeighbourFromPercentage(real_t weight,
                                                      size_t index) const;

        /// \brief
        real_t C() const;

        /// \brief the phase difference [-2, 2]
        void setC(real_t c);

        /// \brief value from 0 to 1, does not throw exceptions if value is
        /// outside the allowed domain
        real_t setCPercentage(real_t c);

        /// \brief
        real_t calculateCPercentage(real_t c) const;

        /// \brief
        real_t calculateCFromPercentage(real_t c) const;

        /// \brief
        real_t Amplitude() const;

        /// \brief the amplitude determines influence of a flexor/extensor on
        /// the final output signal [−1, 1]
        void setAmplitude(real_t amplitude);

        /// \brief value from 0 to 1, does not throw exceptions if value is
        /// outside the allowed domain
        real_t setAmplitudePercentage(real_t amplitude);

        /// \brief
        real_t calculateAmplitudePercentage(real_t amplitude) const;

        /// \brief
        real_t calculateAmplitudeFromPercentage(real_t amplitude) const;

        /// \brief
        real_t Offset() const;

        /// \brief the offset dims shape of the final output signal to the
        /// starting position [-0.1, 0.1]
        void setOffset(real_t offset);

        /// \brief value from 0 to 1, does not throw exceptions if value is
        /// outside the allowed domain
        real_t setOffsetPercentage(real_t offset);

        /// \brief
        real_t calculateOffsetPercentage(real_t offset) const;

        /// \brief
        real_t calculateOffsetFromPercentage(real_t offset) const;

        public:
        /// \brief
        const real_t WEIGHT_MIN = 0;

        /// \brief
        const real_t WEIGHT_MAX = 1.0;

        /// \brief
        const real_t C_MIN = -2;

        /// \brief
        const real_t C_MAX = 2;

        /// \brief
        const real_t AMPLITUDE_MIN = -1;

        /// \brief
        const real_t AMPLITUDE_MAX = 1;

        /// \brief
        const real_t OFFSET_MIN = -0.1;

        /// \brief
        const real_t OFFSET_MAX = 0.1;

        protected:

        /// \brief current value
        real_t phi;

        private:
        // RythmGenerationNeuron parameters

        /// \brief the weight that determines the shape [0, 1.0]
        real_t weight;

        /// \brief weights for neighbours nodes
        std::vector<real_t> weight_neigbours;

        /// \brief the phase difference [-2, 2]
        real_t c;

        /// \brief the amplitude determines influence of a flexor/extensor on
        /// the final output signal [−1, 1]
        real_t amplitude;

        /// \brief the offset dims shape of the final output signal to the
        /// starting position [-0.1, 0.1]
        real_t offset;
      };
    }
  }
}


#endif  // REVOLVE_BRAIN_RYTHMGENERATIONNEURON_H
