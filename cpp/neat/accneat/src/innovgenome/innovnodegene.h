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

#ifndef CPP_NEAT_ACCNEAT_SRC_INNOVGENOME_INNOVGENOME_H_
#define CPP_NEAT_ACCNEAT_SRC_INNOVGENOME_INNOVGENOME_H_

#pragma once


#include <assert.h>
#include <iostream>
#include <string>

#include <yaml-cpp/yaml.h>

#include "neat.h"

namespace NEAT
{
  class InnovNodeGene
  {
    /// \brief identify the trait derived by this node
    int trait_id;

    /// \brief
    std::string creator_name;

    /// \brief
    int creator_index;

    public:
    /// \brief When frozen, cannot be mutated
    /// (meaning its trait pointer is fixed)
    bool frozen;

    /// \brief
    nodetype type;

    /// \brief A node can be given an identification number for saving in files
    int node_id;

    InnovNodeGene() : InnovNodeGene("ERROR")
    {}

    InnovNodeGene(const InnovNodeGene &other);

    // Construct InnovNodeGene with invalid state.
    InnovNodeGene(const std::string &robot_name);

    InnovNodeGene(nodetype ntype,
                  int nodeid,
                  const std::string &robot_name,
                  const std::string &creator_name,
                  const int creator_index);

    InnovNodeGene(nodetype ntype,
                  int nodeid,
                  const std::string &robot_name)
            : InnovNodeGene(ntype, nodeid, robot_name, robot_name, -1)
    {}

    ~InnovNodeGene();

    InnovNodeGene &operator=(const InnovNodeGene &other);

    inline void set_trait_id(int id)
    {
      assert(id > 0);
      trait_id = id;
    }

    inline int get_trait_id() const
    {
      return trait_id;
    }

    inline const nodetype get_type() const
    {
      return type;
    }

    inline void set_type(nodetype t)
    {
      type = t;
    }

    inline const std::string &get_creator_name()
    {
      return creator_name;
    }

    inline int get_creator_index()
    {
      return creator_index;
    }

    void set_creator_name(const std::string &creator_name);

    void set_creator_index(int creator_index);

    friend struct YAML::convert<NEAT::InnovNodeGene>;
  };
}  // namespace NEAT

namespace YAML
{
  template <>
  struct convert<NEAT::InnovNodeGene>
  {
    static Node encode(const NEAT::InnovNodeGene &rhs);

    static bool decode(const Node &node,
                       NEAT::InnovNodeGene &rhs);
  };
}

#endif
