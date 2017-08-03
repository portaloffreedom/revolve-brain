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

#ifndef REVOLVEBRAIN_BRAIN_PYTHON_SUPGBRAIN_H_
#define REVOLVEBRAIN_BRAIN_PYTHON_SUPGBRAIN_H_

#include <vector>

#include <boost/python/list.hpp>

#include "brain/SUPGBrain.h"

namespace revolve {
namespace brain {

class SUPGBrain_python
        : public revolve::brain::SUPGBrain
{
public:
    SUPGBrain_python(revolve::brain::EvaluatorPtr evaluator,
                     const boost::python::list /*< boost::python::list< float > >*/ &neuron_coordinates,
                     const boost::python::list /*< ActuatorPtr> */ &motors,
                     const boost::python::list /*< SensorPtr> */ &sensors);


    void
    update(boost::python::list &actuators,
           const boost::python::list &sensors,
           double t,
           double step);

};

}
}

#endif // REVOLVEBRAIN_BRAIN_PYTHON_SUPGBRAIN_H_
