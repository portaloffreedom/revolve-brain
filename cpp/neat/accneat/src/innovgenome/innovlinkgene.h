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

#ifndef _GENE_H_
#define _GENE_H_

#include <string>

#include <yaml-cpp/yaml.h>

#include "neat.h"
#include "trait.h"
#include "network/network.h"

namespace NEAT
{
  class InnovLinkGene
  {
    /// \brief Weight of connection
    real_t _weight;

    /// \brief NNode inputting into the link
    int _in_node_id;

    /// \brief NNode that the link affects
    int _out_node_id;

    /// \brief
    bool _is_recurrent;

    /// \brief identify the trait derived by this link
    int _trait_id;

    /// \brief
    std::string creator_name;

    /// \brief
    int creator_index;

    public:
    inline int out_node_id() const
    {
      return _out_node_id; }

    inline void set_out_node_id(int id)
    { _out_node_id = id; }

    inline int in_node_id() const
    {
      return _in_node_id; }

    inline void set_in_node_id(int id)
    {
      _in_node_id = id; }

    inline real_t &weight()
    {
      return _weight; }

    inline int trait_id() const
    {
      return _trait_id; }

    inline void set_trait_id(int tid)
    {
      _trait_id = tid; }

    inline bool is_recurrent() const
    {
      return _is_recurrent; }

    inline void set_recurrent(bool r)
    {
      _is_recurrent = r; }

    inline const std::string &get_creator_name()
    {
      return creator_name; }

    inline int get_creator_index()
    {
      return creator_index; }

    /// \brief
    int innovation_num;

    /// \brief Used to see how much mutation has changed the link
    real_t mutation_num;

    /// \brief When this is off the InnovLinkGene is disabled
    bool enable;

    /// \brief When frozen, the linkweight cannot be mutated
    bool frozen;

    /// \brief Construct a gene in an invalid default state.
    InnovLinkGene()
    {}

    /// \brief Construct a gene with no trait
    InnovLinkGene(real_t w,
                  int inode_id,
                  int onode_id,
                  bool recur,
                  int innov,
                  real_t mnum,
                  const std::string &creator_name,
                  const int creator_index);

    /// \brief Construct a gene with a trait
    InnovLinkGene(int trait_id,
                  real_t w,
                  int inode_id,
                  int onode_id,
                  bool recur,
                  int innov,
                  real_t mnum,
                  const std::string &creator_name,
                  const int creator_index);

    /// \brief Construct a gene off of another gene as a duplicate
    InnovLinkGene(InnovLinkGene *g,
                  int trait_id,
                  int inode_id,
                  int onode_id);

    /// \brief Copy Constructor
    InnovLinkGene(const InnovLinkGene &gene);

    /// \brief
    ~InnovLinkGene();

    /// \brief
    bool operator==(const InnovLinkGene &rhs) const;

    /// \brief
    friend struct YAML::convert<NEAT::InnovLinkGene>;
  };
}  // namespace NEAT

namespace YAML
{
  template <>
  struct convert<NEAT::InnovLinkGene>
  {
    static Node encode(const NEAT::InnovLinkGene &rhs);

    static bool decode(const Node &node,
                       NEAT::InnovLinkGene &rhs);
  };
}
#endif
