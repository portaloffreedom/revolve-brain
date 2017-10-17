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
* Date: November 9, 2016
*
*/

#ifndef REVOLVE_BRAIN_CPGNETWORK_H
#define REVOLVE_BRAIN_CPGNETWORK_H

#include <limits>
#include <memory>
#include <vector>

#include "MotoNeuron.h"
#include "PatternFormationNeuron.h"
#include "RythmGenerationNeuron.h"

namespace revolve
{
  namespace brain
  {
    namespace cpg
    {
      class CPGNetwork
      {
        public:
        struct Weights
        {
          cpg::real_t we;
          cpg::real_t wf;
        };

        public:
        /// \brief
        CPGNetwork(
                size_t n_sensors,
                size_t n_connections);

        /// \brief
        virtual ~CPGNetwork();

        /// \brief calculates next output of the network
        /// Updates the network to the new steps and returns the next result
        /// \param sensor_readings vector containing sensor readings
        /// \param step time passed since last update
        /// \return revolve::brain::cpg::real_t output for the network after
        /// the update
        real_t update(
                const std::vector< real_t > &sensor_readings,
                double step);

        // GETTERS and SETTERS

        // Genome getter and setters
        /// \brief
        std::shared_ptr< std::vector< real_t>> Genome();

        /// \brief
        const std::shared_ptr< const std::vector< real_t>> get_genome() const;

        /// \brief
        void set_genome(std::vector< real_t >);

        /// \brief
        void update_genome();

        /// \brief
        struct Limit
        {
          real_t lower;
          real_t upper;

          Limit(
                  real_t l,
                  real_t u)
                  : lower(l), upper(u)
          {}

          Limit()
                  : lower(std::numeric_limits< real_t >::lowest())
                  , upper(std::numeric_limits< real_t >::max())
          {}

          Limit &operator=(const Limit &o)
          {
            this->upper = o.upper;
            this->lower = o.lower;
            return *this;
          }
        };

        /// \brief
        const std::vector< Limit > &GenomeLimits();

        /// \brief
        void addConnection(CPGNetwork *new_connection)
        {
          connections.push_back(new_connection);
        }

        /// \brief setting raw values
        void setRGEWeight(real_t value)
        {
          rge->setWeight(value);
        }

        /// \brief
        void setRGFWeight(real_t value)
        {
          rgf->setWeight(value);
        }

        /// \brief
        void setRGEWeightNeighbour(
                real_t value,
                size_t index)
        {
          rge->setWeightNeighbour(value, index);
        }

        /// \brief
        void setRGFWeightNeighbour(
                real_t value,
                size_t index)
        {
          rgf->setWeightNeighbour(value, index);
        }

        /// \brief
        void setRGEC(real_t value)
        {
          rge->setC(value);
        }

        /// \brief
        void setRGFC(real_t value)
        {
          rgf->setC(value);
        }

        /// \brief
        void setRGEAmplitude(real_t value)
        {
          rge->setAmplitude(value);
        }

        /// \brief
        void setRGFAmplitude(real_t value)
        {
          rgf->setAmplitude(value);
        }

        /// \brief
        void setRGEOffset(real_t value)
        {
          rge->setOffset(value);
        }

        /// \brief
        void setRGFOffset(real_t value)
        {
          rgf->setOffset(value);
        }

        /// \brief
        void setPFEAlpha(real_t value)
        {
          pfe->setAlpha(value);
        }

        /// \brief
        void setPFFAlpha(real_t value)
        {
          pff->setAlpha(value);
        }

        /// \brief
        void setPFETheta(real_t value)
        {
          pfe->setTheta(value);
        }

        /// \brief
        void setPFFTheta(real_t value)
        {
          pff->setTheta(value);
        }

        /// \brief setting percentage values
        void setRGEWeightPercentage(real_t value)
        {
          rge->setWeightPercentage(value);
        }

        /// \brief
        void setRGFWeightPercentage(real_t value)
        {
          rgf->setWeightPercentage(value);
        }

        /// \brief
        void setRGEWeightNeighbourPercentage(
                real_t value,
                size_t index)
        {
          rge->setWeightNeighbourPercentage(value, index);
        }

        /// \brief
        void setRGFWeightNeighbourPercentage(
                real_t value,
                size_t index)
        {
          rgf->setWeightNeighbourPercentage(value, index);
        }

        /// \brief
        void setRGECPercentage(real_t value)
        {
          rge->setCPercentage(value);
        }

        /// \brief
        void setRGFCPercentage(real_t value)
        {
          rgf->setCPercentage(value);
        }

        /// \brief
        void setRGEAmplitudePercentage(real_t value)
        {
          rge->setAmplitudePercentage(value);
        }

        /// \brief
        void setRGFAmplitudePercentage(real_t value)
        {
          rgf->setAmplitudePercentage(value);
        }

        /// \brief
        void setRGEOffsetPercentage(real_t value)
        {
          rge->setOffsetPercentage(value);
        }

        /// \brief
        void setRGFOffsetPercentage(real_t value)
        {
          rgf->setOffsetPercentage(value);
        }

        /// \brief
        void setPFEAlphaPercentage(real_t value)
        {
          pfe->setAlphaPercentage(value);
        }

        /// \brief
        void setPFFAlphaPercentage(real_t value)
        {
          pff->setAlphaPercentage(value);
        }

        /// \brief
        void setPFEThetaPercentage(real_t value)
        {
          pfe->setThetaPercentage(value);
        }

        /// \brief
        void setPFFThetaPercentage(real_t value)
        {
          pff->setThetaPercentage(value);
        }

        protected:
        /// \brief
        void updateRythmGeneration(real_t step);

        /// \brief
        void updatePatternFormation(
                const std::vector< real_t > &sensor_readings,
                real_t step);

        /// \brief
        void updateMotoNeuron(real_t step);

        protected:
        /// \brief
        RythmGenerationNeuron *rge;

        /// \brief
        RythmGenerationNeuron *rgf;

        /// \brief
        PatternFormationNeuron *pfe;

        /// \brief
        PatternFormationNeuron *pff;

        /// \brief
        MotoNeuron *mn;

        /// \brief
        real_t rge_out;

        /// \brief
        real_t rgf_out;

        /// \brief
        real_t pfe_out;

        /// \brief
        real_t pff_out;

        /// \brief
        real_t mn_out;

        /// \brief
        const size_t n_connections;

        /// \brief
        std::vector< CPGNetwork * > connections;

        /// \brief
        std::shared_ptr< std::vector< real_t>> genome;

        /// \brief
        std::vector< Limit > genome_limits;
      };
    }
  }
}


#endif  // REVOLVE_BRAIN_CPGNETWORK_H
