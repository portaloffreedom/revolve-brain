#include "spline_rlpower_learner.h"

using namespace revolve::brain;

base_controller* spline_rlpower_learner::update(const std::vector<SensorPtr>& sensors,
                                                          double t, double step)
{
    //TODO temper with `this->active_controller`
    //return base_learner::update(sensors, t, step);
    return base_rlpower_learner::update(sensors, t, step);
}
