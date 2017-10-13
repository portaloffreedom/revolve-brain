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

#ifndef REVOLVEBRAIN_BRAIN_CONVERSION_H_
#define REVOLVEBRAIN_BRAIN_CONVERSION_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "brain/controller/RafCPGController.h"
#include "brain/controller/LayeredExtCPPN.h"
#include "brain/learner/NEATLearner.h"
#include "brain/learner/RLPowerLearner.h"
#include "brain/learner/cppneat/GeneticEncoding.h"

namespace revolve
{
  namespace brain
  {
    /// \brief used for communication between cppneat learner and ext nn net
    /// controller the conversion methods work only when using standard neat
    extern std::map<cppneat::Neuron::Ntype, cppneat::Neuron::NeuronTypeSpec>
            brain_spec;

    extern std::map<int, size_t> InputMap;

    extern std::map<int, size_t> OutputMap;

    void SetBrainSpec(bool _isHyperNeat);

    CPPNConfigPtr convertForController(cppneat::GeneticEncodingPtr _genotype);

    cppneat::GeneticEncodingPtr convertForLearner(CPPNConfigPtr _config);

///////////////////////////////////////////////////////////////////////////////
/// HyperNEAT_CPG
///////////////////////////////////////////////////////////////////////////////
    /// \brief used for communication between rlpower learner and
    /// ext nn weights controller
    std::vector<double> convertPolicyToDouble(PolicyPtr _genotype);

    PolicyPtr convertDoubleToNull(std::vector<double> _phenotype);
///////////////////////////////////////////////////////////////////////////////

    /// \brief used for communication between hyperneat learner and
    /// ext nn net controller
    extern boost::shared_ptr<CPPNConfig> RafCpgNetwork;

    extern std::map<std::string, std::tuple<int, int, int>> neuron_coordinates;

    extern cppneat::GeneticEncodingPtr last_genotype_;

    /// \brief converts a layered genotype to a layered phenotype only works if
    /// genotype->layered == true
    boost::shared_ptr<LayeredExtNNConfig>
    convertForLayeredExtNN(cppneat::GeneticEncodingPtr genotype);

///////////////////////////////////////////////////////////////////////////////
/// RLPower_CPG~RLPower_CPPN
///////////////////////////////////////////////////////////////////////////////
    boost::shared_ptr<CPPNConfig>
    convertGEtoNN(cppneat::GeneticEncodingPtr genotype);

    cppneat::GeneticEncodingPtr
    convertNNtoGE(boost::shared_ptr<CPPNConfig> config);
///////////////////////////////////////////////////////////////////////////////

    /// \brief used for communication between spline controller and hyperneat
    /// learner contains the coordinates of the actuators matching the order the
    /// actuators are given in the update method coordinate of actuators[0] is
    /// in sorted_coordinates[0]
    extern std::vector<std::pair<int, int>> sorted_coordinates;

    extern size_t spline_size;

    extern size_t update_rate;

    extern size_t cur_step;

    /// \brief returns the starting network for hyperneat on splines
    cppneat::GeneticEncodingPtr HyperNeatSplines();

    PolicyPtr
    convertForSplinesFromHyper(cppneat::GeneticEncodingPtr _genotype);

    cppneat::GeneticEncodingPtr convertForHyperFromSplines(PolicyPtr policy);
  }
}


#endif  //  REVOLVEBRAIN_BRAIN_CONVERSION_H_
