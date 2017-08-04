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

#ifndef REVOLVEBRAIN_BRAIN_CONTROLLER_POLICYCONTROLLER_H_
#define REVOLVEBRAIN_BRAIN_CONTROLLER_POLICYCONTROLLER_H_

#include <vector>

#include "Controller.h"

namespace revolve
{
  namespace brain
  {
    typedef std::vector<double> Spline;

    typedef std::vector<Spline> Policy;

    typedef std::shared_ptr<Policy> PolicyPtr;

    class PolicyController
            : public Controller<PolicyPtr>
    {
      public:
      /// \brief  = 5 seconds
      static const double CYCLE_LENGTH;

      /// \brief = 100 points
      static const size_t INTERPOLATION_CACHE_SIZE;

      /// \brief Constructor
      explicit PolicyController(size_t n_actuators,
                                size_t interpolation_cache_size);

      /// \brief Constructor
      explicit PolicyController(size_t n_actuators);

      /// \brief Destructor
      virtual ~PolicyController() override;

      /// \brief
      void update(const std::vector<ActuatorPtr> &actuators,
                  const std::vector<SensorPtr> &sensors,
                  double t,
                  double step) override;

      /// \brief
      void generateOutput(const double time,
                          double *output_vector);

      /// \brief
      PolicyPtr getPhenotype() override;

      /// \brief
      void setPhenotype(PolicyPtr policy) override;

      /// \brief Generate cache policy
      void update_cache();

      // STATIC METHODS -----

      /// \brief
      static void InterpolateCubic(Policy *const source_y,
                                   Policy *destination_y);

      /// \brief
      static PolicyController
      *GenerateRandomController(double noise_sigma,
                                size_t n_actuators,
                                size_t n_spline_points,
                                size_t interpolation_cache_size);

      /// \brief
      static PolicyController *GenerateRandomController(double noise_sigma,
                                                        size_t n_actuators,
                                                        size_t n_spline_points)
      {
        return GenerateRandomController(noise_sigma,
                                        n_actuators,
                                        n_spline_points,
                                        INTERPOLATION_CACHE_SIZE);
      }

      protected:
      /// \brief number of actuators the controller is expecting to send signal
      const size_t n_actuators_;

      /// \brief number of `interpolation_cache` data points
      const size_t interpolation_cache_size_;

      /// \brief pointer to the current policy
      PolicyPtr policy_;

      /// \brief pointer to the interpolated current_policy_
      /// (default 100 points)
      PolicyPtr interpolation_cache_;

      /// \brief start time of one cycle from which we count
      double cycle_start_time_;
    };
  }
}

#endif  //  REVOLVEBRAIN_BRAIN_CONTROLLER_POLICYCONTROLLER_H_
