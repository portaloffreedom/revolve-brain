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

#include "SUPGBrainPhototaxis.h"

#include <boost/make_shared.hpp>
#include <sstream>

using namespace revolve::brain;

const std::vector<SensorPtr> SUPGBrainPhototaxis::PushCombinedLightSensor(
        const std::vector<SensorPtr>& sensors,
        const SensorPtr combined_light_sensor)
{
    std::vector<SensorPtr> sensors_copy(sensors);

    if (!combined_light_sensor)
        sensors_copy.push_back(boost::make_shared<CombinedLightSensor>("fake_combined_sensor_filler"));
    else
        sensors_copy.push_back(combined_light_sensor);

    return sensors_copy;
}

SUPGBrainPhototaxis::SUPGBrainPhototaxis(const std::string &robot_name,
                                         EvaluatorPtr evaluator,
                                         std::function<boost::shared_ptr<FakeLightSensor> (std::vector<float> coordinates)> _light_constructor_left,
                                         std::function<boost::shared_ptr<FakeLightSensor> (std::vector<float> coordinates)> _light_constructor_right,
                                         double _light_radius_distance,
                                         const std::vector< std::vector< float > >& neuron_coordinates,
                                         const std::vector< ActuatorPtr >& actuators,
                                         const std::vector< SensorPtr >& sensors,
                                         const PHASE testing_phase)
    : SUPGBrain(robot_name, std::move(evaluator), neuron_coordinates, actuators, PushCombinedLightSensor(sensors))
    , TESTING_PHASE(testing_phase)
    , current_phase(END)
    , light_constructor_left(std::move(_light_constructor_left))
    , light_constructor_right(std::move(_light_constructor_right))
    , current_light_left(nullptr)
    , current_light_right(nullptr)
    , light_radius_distance(_light_radius_distance)
    , partial_fitness(0)
    , grace_done(false)
    , racing_done(false)
    , combined_light_sensor(boost::make_shared<CombinedLightSensor>("combined_light_sensor"))
{
}

void SUPGBrainPhototaxis::update(const std::vector< ActuatorPtr >& actuators,
                                 const std::vector< SensorPtr >& sensors,
                                 double t, double step)
{
    std::vector<SensorPtr> sensors_enhanced = SUPGBrainPhototaxis::PushCombinedLightSensor(sensors, combined_light_sensor);
    this->learner(t);
    SUPGBrain::controller<const std::vector< ActuatorPtr >, const std::vector< SensorPtr >>(actuators, sensors_enhanced, t, step);
}

SUPGBrainPhototaxis::SUPGBrainPhototaxis(EvaluatorPtr evaluator)
        : SUPGBrain(evaluator)
        , TESTING_PHASE(CENTER)
        , grace_done(false)
        , racing_done(false)
{
}

double SUPGBrainPhototaxis::getFitness()
{
    return partial_fitness;
}


#define MAX_PHASE_FITNESS 100
double SUPGBrainPhototaxis::getPhaseFitness()
{
    double left_eye = current_light_left == nullptr ? std::numeric_limits<double>::min() :
                      current_light_left->read();
    double right_eye = current_light_right == nullptr ? std::numeric_limits<double>::min() :
                       current_light_right->read();

    double value = (left_eye + right_eye)
         - ((left_eye - right_eye) * (left_eye - right_eye));

//     double value = (left_eye + right_eye)
//          - std::fabs(left_eye - right_eye);

    if (value > MAX_PHASE_FITNESS)
        return MAX_PHASE_FITNESS;

    return value;
}
#undef MAX_PHASE_FITNESS

void SUPGBrainPhototaxis::learner(double t)
{
    if (this->isOffline())
        return;

    // Evaluate policy on certain time limit
    if ((t-start_eval_time) > SUPGBrain::FREQUENCY_RATE)
    {
        // check if to stop the experiment. Negative value for MAX_EVALUATIONS will never stop the experiment
        if (SUPGBrain::MAX_EVALUATIONS > 0 && evaluation_counter > SUPGBrain::MAX_EVALUATIONS) {
            std::cout << "Max Evaluations (" << SUPGBrain::MAX_EVALUATIONS << ") reached. stopping now." << std::endl;
            std::exit(0);
        }


        // FITNESS update
        if (current_phase != END) {
            double left_eye = current_light_left == nullptr ? std::numeric_limits<double>::max() :
                              current_light_left->light_distance();
            double right_eye = current_light_right == nullptr ? std::numeric_limits<double>::max() :
                               current_light_right->light_distance();

            // HACK: 4 is the base value of fitness if the robot didn't move in respect to the light.
            // This hardcoded number is to offset the value of the fitness to a 0 in case of the robot didn't move.
            // The value will be negative in case the robot moved away from the target.
            double phase_fitness = getPhaseFitness();
            std::cout << "SUPGBrainPhototaxis::learner - partial fitness[" << current_phase << "]: " << phase_fitness
                      << " light_distance_left: " << left_eye
                      << " light_distance_right: " << right_eye
                      << std::endl;
            partial_fitness += phase_fitness;
        }

        // Advance Phase
        switch (current_phase) {
            case END: // code will execute this case only for the initialization
                std::cout << "SUPGBrainPhototaxis::learner - INIT!" << std::endl;
                break;
            case CENTER:
            case LEFT:
            case MORELEFT:
            case RIGHT:
            case MORERIGHT:
                current_phase = END;
                break;
            default:
                std::ostringstream error_ss;
                error_ss << "ERROR! THIS PHASE (" << current_phase << ") SHOULD NOT BE CALLED!";
                std::string error = error_ss.str();
                std::cout << error << std::endl;
                throw std::runtime_error(error);
                current_phase = this->TESTING_PHASE;
                break;
            /*
            case CENTER:    phase = LEFT;      break;
            case LEFT:      phase = RIGHT;     break;
            case RIGHT:     phase = MORELEFT;  break;
            case MORELEFT:  phase = MORERIGHT; break;
            case MORERIGHT: phase = END;       break;
            */
        }

        // If phase is `END`, start a new phase
        if (current_phase == END) {
            std::cout << "SUPGBrainPhototaxis::learner - finished with fitness: "
                      << getFitness()
                      << " general gait fitness: "
                      << evaluator->fitness() * 100 * 30 // fitness is reported as m/s over 30 seconds.
                      << std::endl;

            evaluation_counter++;
            this->nextBrain();
            partial_fitness = 0;

            std::cout << "SUPGBrainPhototaxis::learner - NEW BRAIN (generation " << evaluation_counter << " ; generation " << neat->getGeneration() << "  )" << std::endl;

            // initial phase where to start
            current_phase = this->TESTING_PHASE;

            // memory must be initialized -- value not really important
            this->setLightCoordinates(current_phase);
        }

        // evaluation restart
        start_eval_time = t;
        this->grace_done = false;
        this->racing_done = false;
        evaluator->start();
        //std::cout << "Grace Period start: " << t << std::endl;
    }

    // grace period passed: //2 seconds of grace period
    #define GRACE_PERIOD 2
    if (not this->grace_done && (t-start_eval_time) > GRACE_PERIOD) {

        // check if to stop the experiment. Negative value for MAX_EVALUATIONS will never stop the experiment
        if (SUPGBrain::MAX_EVALUATIONS > 0 && evaluation_counter > SUPGBrain::MAX_EVALUATIONS) {
            std::cout << "Max Evaluations (" << SUPGBrain::MAX_EVALUATIONS << ") reached. stopping now." << std::endl;
            std::exit(0);
        }

        //std::cout << "Grace Period end: " << t << std::endl;
        // reposition learner lights
        // END PHASE SHOULD NOT BE POSSIBLE HERE!
        this->setLightCoordinates(current_phase);

        this->grace_done = true;
    }

    // racing evaluation: //5 seconds of racing evaluation
    #define RACING_PERIOD 5
    if (not this->racing_done && (t-start_eval_time) > RACING_PERIOD) {

        //std::cout << "RACING Period end: " << t << std::endl;
        // END PHASE SHOULD NOT BE POSSIBLE HERE!
        double distance_raced = evaluator->fitness() * 100 * 30; // fitness is reported as m/s over 30 seconds.
        double distance_to_race; //cm
        if (evaluation_counter < 500) {
            distance_to_race = 0; //cm
        } else if (evaluation_counter >= 500 && evaluation_counter < 1000) {
            distance_to_race = 1; //cm
        } else if (evaluation_counter >= 1000 && evaluation_counter < 1500) {
            distance_to_race = 3; //cm
        } else if (evaluation_counter >= 1500 && evaluation_counter < 2000) {
            distance_to_race = 5; //cm
        } else {
            distance_to_race = 9; //cm
        }

        std::cout << "RACING: evaluating distance_raced(" << distance_raced << ") and distance_to_race(" << distance_to_race << ')' << std::endl;
        if (distance_raced < distance_to_race) {
            std::cout << "RACING Failed, starting new round" << std::endl;

            // kill the individual
            partial_fitness = 0;

            // and start a new one
            evaluation_counter++;
            this->nextBrain();

            std::cout << "SUPGBrainPhototaxis::learner - NEW BRAIN (evaluation " << evaluation_counter << " ; generation " << neat->getGeneration() << "  )" << std::endl;
            // initial phase where to start
            current_phase = this->TESTING_PHASE;

            // evaluation restart
            start_eval_time = t;
            this->grace_done = false;
            this->racing_done = false;
            evaluator->start();
            //std::cout << "Grace Period start: " << t << std::endl;
        } else {
            this->racing_done = true;
        }
    }
}

void SUPGBrainPhototaxis::setLightCoordinates(SUPGBrainPhototaxis::PHASE phase)
{
    std::vector<float> relative_coordinates;

    static const double pi = std::acos(-1);
    static const double angle_15 = pi/12;
    static const double angle_52_5 = 7*pi/24;

    const double radius = light_radius_distance;
    const float x_52_5 = (const float) (std::cos(angle_52_5) * radius);
    const float y_52_5 = (const float) (std::sin(angle_52_5) * radius);
    const float x_15   = (const float) (std::cos(angle_15) * radius);
    const float y_15   = (const float) (std::sin(angle_15) * radius);

    switch (phase) {
    case CENTER:
        relative_coordinates = {0, -static_cast<float>(radius)};
        break;
    case LEFT:
        relative_coordinates = {-x_52_5, -y_52_5};
        break;
    case RIGHT:
        relative_coordinates = {x_52_5, -y_52_5};
        break;
    case MORELEFT:
        relative_coordinates = {-x_15, -y_15};
        break;
    case MORERIGHT:
        relative_coordinates = {x_15, -y_15};
        break;
    default:
        std::cerr << "PLEASE USE A VALID LIGHT COORDINATE PHASE!" << std::endl;
        throw std::runtime_error("PLEASE USE A VALID LIGHT COORDINATE PHASE!");
    }


    this->setLightCoordinates(relative_coordinates);
}

void SUPGBrainPhototaxis::setLightCoordinates(const std::vector<float> &relative_coordinates)
{
    if (current_light_left)
        current_light_left->replace(light_constructor_left(relative_coordinates));
    else
        current_light_left = light_constructor_left(relative_coordinates);
    if (current_light_right)
        current_light_right->replace(light_constructor_right(relative_coordinates));
    else
        current_light_right = light_constructor_right(relative_coordinates);
    combined_light_sensor->set_sensor_left(current_light_left);
    combined_light_sensor->set_sensor_right(current_light_right);
}

SUPGBrainPhototaxis::CombinedLightSensor::CombinedLightSensor(const std::string &id)
        : id(id)
{
}

void SUPGBrainPhototaxis::CombinedLightSensor::read(double *input_vector)
{
    if (this->light_sensor_left && this->light_sensor_right) {
        double sensor_left = 0;
        double sensor_right = 0;
        light_sensor_left->read(&sensor_left);
        light_sensor_right->read(&sensor_right);

        *input_vector = sensor_right - sensor_left; //order doesn't matter if it's consistent
    } else {
        *input_vector = 0;
    }
}

unsigned int SUPGBrainPhototaxis::CombinedLightSensor::inputs() const
{
    return 1;
}

std::string SUPGBrainPhototaxis::CombinedLightSensor::sensorId() const
{
    return "Combined sensor light sensor id: " + this->id;
}

void SUPGBrainPhototaxis::CombinedLightSensor::set_sensor_left(revolve::brain::SensorPtr light_sensor_left)
{
    if (!light_sensor_left || light_sensor_left->inputs() < 1) {
        throw std::runtime_error("invalid sensor for combined sensor");
    }

    this->light_sensor_left = light_sensor_left;
}

void SUPGBrainPhototaxis::CombinedLightSensor::set_sensor_right(revolve::brain::SensorPtr light_sensor_right)
{
    if (!light_sensor_right || light_sensor_right->inputs() < 1) {
        throw std::runtime_error("invalid sensor for combined sensor");
    }

    this->light_sensor_right = light_sensor_right;
}
