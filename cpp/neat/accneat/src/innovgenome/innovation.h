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

/*
  Copyright 2001 The University of Texas at Austin

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifndef _INNOVATION_H_
#define _INNOVATION_H_

#include <map>
#include <functional>
#include <vector>

#include "neat.h"

namespace NEAT
{
  enum innovtype
  {
    NEWNODE = 0,
    NEWLINK = 1
  };

  class InnovationId
  {
    public:
    /// \brief
    innovtype innovation_type;

    /// \brief
    int node_in_id;

    /// \brief
    int node_out_id;

    /// \brief
    int old_innov_num;

    /// \brief
    bool recur_flag;

    /// \brief Node
    InnovationId(
            int nin,
            int nout,
            int oldinnov);

    /// \brief Link
    InnovationId(
            int nin,
            int nout,
            bool recur);

    bool operator<(const InnovationId &other) const;

    bool operator==(const InnovationId &other) const;
  };

  class InnovationParms
  {
    public:
    /// \brief
    real_t new_weight;

    /// \brief
    int new_trait_id;

    InnovationParms();

    InnovationParms(
            real_t w,
            int t);
  };

  class IndividualInnovation
  {
    public:
    typedef std::function< void(const class Innovation *innov) > ApplyFunc;

    /// \brief
    int population_index;

    /// \brief
    InnovationId id;

    /// \brief
    InnovationParms parms;

    /// \brief
    ApplyFunc apply;

    IndividualInnovation(
            int population_index_,
            InnovationId id_,
            InnovationParms parms_,
            ApplyFunc apply_);
  };

  typedef std::function< void(
          InnovationId id,
          InnovationParms parms,
          IndividualInnovation::ApplyFunc func) >
          CreateInnovationFunc;

  class Innovation
  {
    public:
    /// \brief
    InnovationId id;

    /// \brief
    InnovationParms parms;

    /// \brief The number assigned to the innovation
    int innovation_num1;

    /// \brief If this is a new node innovation, then there are 2 innovations
    /// (links) added for the new node
    int innovation_num2;

    /// \brief If a new node was created, this is its node_id
    int newnode_id;

    /// \brief Link
    Innovation(
            InnovationId id_,
            InnovationParms parms_,
            int innovation_num1_);

    /// \brief Node
    Innovation(
            InnovationId id_,
            InnovationParms parms_,
            int innovation_num1_,
            int innovation_num2_,
            int newnode_id_);
  };

  class PopulationInnovations
  {
    /// \brief For holding the genetic innovations of the newest generation
    std::vector< std::vector< IndividualInnovation>> innovations;

    /// \brief
    std::map< InnovationId, std::vector< IndividualInnovation>> id2inds;

    /// \brief
    int cur_node_id;

    /// \brief
    int cur_innov_num;

    public:
    void init(
            int node_id,
            int innov_num);

    void add(const IndividualInnovation &innov);

    void apply();
  };
}  // namespace NEAT

#endif
