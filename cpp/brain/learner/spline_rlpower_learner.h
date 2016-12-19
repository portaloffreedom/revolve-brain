#ifndef REVOLVE_BRAIN_SPLINE_RLPOWER_LEARNER_H
#define REVOLVE_BRAIN_SPLINE_RLPOWER_LEARNER_H

#include "base_rlpower_learner.h"
#include "brain/controller/spline_controller.h"

namespace revolve {
namespace brain {

class spline_rlpower_learner : public revolve::brain::base_rlpower_learner
{
public:
    virtual base_controller* update(const std::vector<SensorPtr> &sensors,
                                    double t, double step) override;
};


}
}

#endif // REVOLVE_BRAIN_SPLINE_RLPOWER_LEARNER_H
