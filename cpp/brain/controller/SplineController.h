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
*
*/

#ifndef REVOLVEBRAIN_BRAIN_CONTROLLER_SPLINECONTROLLER_H_
#define REVOLVEBRAIN_BRAIN_CONTROLLER_SPLINECONTROLLER_H_

#include <vector>

#include "BaseController.h"

namespace revolve
{
  namespace brain
  {
    class BaseLearner;

    class SplineController
            : public BaseController
    {
      public:  // typedefs
      typedef std::vector<double> Spline;
      typedef std::vector<Spline> Policy;
      typedef std::shared_ptr<Policy> PolicyPtr;

      friend class BaseLearner;

      // METHODS-FUNCTIONS -----------------------------------------------------
      public:  // STATIC METHODS
      /// \brief Generate interpolated spline based on number of sampled control
      /// points in 'source_y'
      /// \param source_y: set of control points over which interpolation is
      /// generated
      /// \param destination_y: set of interpolated control points
      /// (default 100 points)
      static void Interpolate_cubic(Policy *const source_y,
                                    Policy *destination_y);

      /// \brief
      static SplineController *
      GenerateRandomController(double noise_sigma,
                                unsigned int n_actuators,
                                unsigned int n_spline_points,
                                unsigned int interpolation_cache_size);

      /// \brief
      static SplineController *
      GenerateRandomController(double noise_sigma,
                                unsigned int n_actuators,
                                unsigned int n_spline_points)
      {
        return GenerateRandomController(noise_sigma,
                                        n_actuators,
                                        n_spline_points,
                                        INTERPOLATION_CACHE_SIZE);
      }

      public:  // methods
      /// \brief
      explicit SplineController(unsigned int n_actuators,
                                unsigned int n_spline_points,
                                unsigned int interpolation_cache_size);

      /// \brief
      explicit SplineController(unsigned int n_actuators,
                                unsigned int n_spline_points);

      /// \brief
      virtual ~SplineController();

      /// \brief
      virtual void update(const std::vector<ActuatorPtr> &actuators,
                          const std::vector<SensorPtr> &sensors,
                          double t,
                          double step) override;

      /// \brief
      virtual void generateOutput(const double time,
                                  double *output_vector);

      /// \brief Generate cache policy
      void update_cache();

      // VARIABLES-CONSTANTS ---------------------------------------------------
      public:  // STATIC CONSTANTS
      /// \brief cycle lenght in seconds = 5 seconds
      static const double CYCLE_LENGTH;
      /// \brief  Default value for the number of `interpolation_cache` data
      /// points (default value for `interpolation_cache_size`) = 100
      static const unsigned int INTERPOLATION_CACHE_SIZE;

      protected:  // consts
      /// \brief number of actuators the spline is expeting to write into
      const unsigned int n_actuators;

      /// \brief number of spline points
      const unsigned int n_spline_points;

      /// \brief Number of `interpolation_cache` data points
      const unsigned int interpolation_cache_size;

      protected:  // variables
      /// \brief Pointer to the current policy
      PolicyPtr policy;

      /// \brief Pointer to the interpolated current_policy_
      /// (default 100 points)
      PolicyPtr interpolation_cache = nullptr;

      /// \brief
      double cycle_start_time;
    };
  }
}

#endif  //  REVOLVEBRAIN_BRAIN_CONTROLLER_SPLINECONTROLLER_H_
