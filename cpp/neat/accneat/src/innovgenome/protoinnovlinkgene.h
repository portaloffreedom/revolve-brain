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

#ifndef CPP_NEAT_ACCNEAT_SRC_INNOVGENOME_PROTOINNOVLINKGENE_H_
#define CPP_NEAT_ACCNEAT_SRC_INNOVGENOME_PROTOINNOVLINKGENE_H_

#pragma once

namespace NEAT
{
  class ProtoInnovLinkGene
  {
    InnovGenome *_genome = nullptr;
    // TODO: does this have to be a InnovLinkGene* now?
    InnovLinkGene *_gene = nullptr;
    InnovNodeGene *_in = nullptr;
    InnovNodeGene *_out = nullptr;
    public:
    void set_gene(InnovGenome *genome,
                  InnovLinkGene *gene)
    {
      _genome = genome;
      _gene = gene;
    }

    InnovLinkGene *gene()
    {
      return _gene;
    }

    void set_out(InnovNodeGene *out)
    {
      _out = out;
      _gene->set_out_node_id(out->node_id);
    }

    InnovNodeGene *out()
    {
      return _out ? _out : _genome->get_node(_gene->out_node_id());
    }

    void set_in(InnovNodeGene *in)
    {
      _in = in;
      _gene->set_in_node_id(in->node_id);
    }

    InnovNodeGene *in()
    {
      return _in ? _in : _genome->get_node(_gene->in_node_id());
    }
  };
}

#endif
