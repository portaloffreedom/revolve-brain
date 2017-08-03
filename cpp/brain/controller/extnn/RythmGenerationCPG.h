/*
 * Copyright (C) 2015-2017 Vrije Universiteit Amsterdam
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
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
 * Date: August 2, 2017.
 *
 */

#ifndef TOL_REVOLVE_RYTHMGENERATIONCPG_H
#define TOL_REVOLVE_RYTHMGENERATIONCPG_H

#include "ENeuron.h"
#include "NeuralConnection.h"

namespace revolve
{
  namespace brain
  {

    class RythmGenerationCPG
            : public Neuron
    {
      public:
      /// \brief Constructor for differential cpg
      /// \param id: string to identify the neuron
      /// \param params: parameters that specify the behavior of the neuron
      /// \return pointer to the differential cpg
      RythmGenerationCPG(const std::string &id,
                         const std::map<std::string, double> &params);

      /// \brief Method to calculate the output of the neuron
      /// \param t: current time
      /// \return the output of the neuron at time t
      double CalculateOutput(double t) override;

      std::map<std::string, double> getNeuronParameters() override;

      void setNeuronParameters(std::map<std::string, double> params) override;

      double Phase() override;

      std::string getType() override;

      /////////////////////////////////////////////////////////////////////////
      public:
      const double WEIGHT_MIN = 0;
      const double WEIGHT_MAX = 1.0;
      const double C_MIN = -2;
      const double C_MAX =  2;
      const double AMPLITUDE_MIN = -1;
      const double AMPLITUDE_MAX =  1;
      const double OFFSET_MIN = -0.1;
      const double OFFSET_MAX =  0.1;

      /// \brief phase difference [-2, 2]
      protected: double bias_;

      /// \brief last time the output was calculated
      protected: double lastTime_;

      /// \brief current value
      protected: double phi;

      /// \brief weight that determines the shape [0, 1.0]
      protected: double weight;

      /// \brief weights for neighbours nodes
      protected: std::vector<double> weight_neigbours;

      /// \brief amplitude determines influence of a flexor/extensor on the
      /// final output signal [−1, 1]
      protected: double amplitude;

      /// \brief offset dims shape of the final output signal to the starting
      /// position [-0.1, 0.1]
      protected: double offset;

    };

  }
}

#endif  // TOL_REVOLVE_RYTHMGENERATIONCPG_H
