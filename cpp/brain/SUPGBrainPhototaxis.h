/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2016  Matteo De Carlo <matteo.dek@covolunablu.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef REVOLVE_BRAIN_SUPGBRAINPHOTOTAXIS_H
#define REVOLVE_BRAIN_SUPGBRAINPHOTOTAXIS_H

#include "FakeLightSensor.h"
#include "SUPGBrain.h"

#include <functional>
#include <boost/shared_ptr.hpp>

namespace revolve {
namespace brain {

class SUPGBrainPhototaxis : protected SUPGBrain
{
  public:
  enum PHASE
  {
      CENTER = 0,
      LEFT = 1,
      MORELEFT = 2,
      RIGHT = 3,
      MORERIGHT = 4,
      END = 5,
  };

  /**
   * Light sensor constructors are passed the coordinates with already the offset included
   */
  SUPGBrainPhototaxis(const std::string &robot_name,
                      EvaluatorPtr evaluator,
                      std::function<boost::shared_ptr<FakeLightSensor>(std::vector<float> coordinates)> _light_constructor_left,
                      std::function<boost::shared_ptr<FakeLightSensor>(std::vector<float> coordinates)> _light_constructor_right,
                      double light_radius_distance,
                      const std::vector<std::vector<float> > &neuron_coordinates,
                      const std::vector<ActuatorPtr> &actuators,
                      const std::vector<SensorPtr> &sensors,
                      const PHASE testing_phase);

  using SUPGBrain::update;
  virtual void update(const std::vector<ActuatorPtr> &actuators,
                      const std::vector<SensorPtr> &sensors,
                      double t, double step) override;

  void setLightCoordinates(PHASE phase);

  virtual void setLightCoordinates(const std::vector<float> &relative_coordinates);

  protected:
  SUPGBrainPhototaxis(EvaluatorPtr evaluator);

  virtual double getFitness() override;

  virtual double getPhaseFitness();

  virtual void learner(double t) override;

  static const std::vector<SensorPtr> PushCombinedLightSensor(
          const std::vector<SensorPtr> &sensors,
          const SensorPtr combined_light_sensor = nullptr);

  //// Templates ---------------------------------------------------------

  const PHASE TESTING_PHASE;
  PHASE current_phase;

  std::function<boost::shared_ptr<FakeLightSensor>(std::vector<float> coordinates)>
          light_constructor_left,
          light_constructor_right;

  boost::shared_ptr<FakeLightSensor> current_light_left, current_light_right;

  double light_radius_distance;
  double partial_fitness;
  bool grace_done;
  bool racing_done;

  // Additional combined sensor class
  class CombinedLightSensor : public revolve::brain::Sensor
  {
  public:
      explicit CombinedLightSensor(const std::string &id);

      virtual void read(double *input_vector) override;
      virtual unsigned int inputs() const override;
      virtual std::string sensorId() const override;

      void set_sensor_left(revolve::brain::SensorPtr light_sensor_left);
      void set_sensor_right(revolve::brain::SensorPtr light_sensor_right);
  private:
      const std::string id;
      revolve::brain::SensorPtr light_sensor_left;
      revolve::brain::SensorPtr light_sensor_right;
  };

  boost::shared_ptr<CombinedLightSensor> combined_light_sensor;
};

}
}

#endif // REVOLVE_BRAIN_SUPGBRAINPHOTOTAXIS_H
