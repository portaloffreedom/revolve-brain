#ifndef REVOLVE_BRAIN_BASE_RLPOWER_LEARNER_H
#define REVOLVE_BRAIN_BASE_RLPOWER_LEARNER_H

#include "base_learner.h"

namespace revolve {
namespace brain {

class base_rlpower_learner : public revolve::brain::base_learner
{
public:
    virtual base_controller* update(const std::vector<SensorPtr> &sensors,
                                    double t, double step) override;
};


}
}

#endif // REVOLVE_BRAIN_BASE_RLPOWER_LEARNER_H
