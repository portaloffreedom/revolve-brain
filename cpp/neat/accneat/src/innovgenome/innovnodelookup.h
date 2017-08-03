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

#pragma once

#include "innovnodegene.h"
#include <vector>
#include <algorithm>

namespace NEAT {

inline bool
nodelist_cmp(const InnovNodeGene &a,
             const InnovNodeGene &b)
{
  return a.node_id < b.node_id;
}

inline bool
nodelist_cmp_key(const InnovNodeGene &node,
                 int node_id)
{
  return node.node_id < node_id;
}

class InnovNodeLookup
{
    std::vector<InnovNodeGene> &nodes;
public:
    // Must be sorted by node_id in ascending order
    InnovNodeLookup(std::vector<InnovNodeGene> &nodes_)
            :
            nodes(nodes_)
    {
    }

    InnovNodeGene *
    find(int node_id)
    {
      auto it = std::lower_bound(nodes.begin(),
                                 nodes.end(),
                                 node_id,
                                 nodelist_cmp_key);
      if (it == nodes.end())
        return nullptr;

      InnovNodeGene &node = *it;
      if (node.node_id != node_id)
        return nullptr;

      return &node;
    }

    InnovNodeGene *
    find(InnovNodeGene *n)
    {
      return find(n->node_id);
    }
};

}
