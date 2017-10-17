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

#include <list>
#include <memory>
#include <vector>

#include "multiinnovgenome.h"

using namespace NEAT;

MultiInnovGenome::MultiInnovGenome(
        std::list< std::unique_ptr< InnovGenome >,
                   std::allocator< std::unique_ptr< InnovGenome>> > &genome_list)
{
  innov_genome_list = new std::vector< std::unique_ptr< InnovGenome > >();

  for (auto g = genome_list.begin(); g not_eq genome_list.end(); g++)
  {
    innov_genome_list->push_back(std::move(*g));
  }
  innov_genome_list->shrink_to_fit();
}

MultiInnovGenome::~MultiInnovGenome()
{
  delete innov_genome_list;
}

Genome &MultiInnovGenome::operator=(const Genome &other)
{
  return *this = dynamic_cast<const MultiInnovGenome &>(other);
}

void MultiInnovGenome::verify()
{
#ifdef NDEBUG
  return;
#else
  for (auto g =
          innov_genome_list->begin(); g not_eq innov_genome_list->end(); g++)
  {
    (*g)->verify();
  }
#endif
}

Genome::Stats MultiInnovGenome::get_stats()
{
  size_t nodes_size = 0,
          links_size = 0;
  for (auto g =
          innov_genome_list->begin(); g not_eq innov_genome_list->end(); g++)
  {
    nodes_size += (*g)->nodes.size();
    links_size += (*g)->links.size();
  }

  return {nodes_size, links_size};
}

void MultiInnovGenome::print(std::ostream &out) const
{
  for (auto g =
          innov_genome_list->begin(); g not_eq innov_genome_list->end(); g++)
  {
    (*g)->print(out);
  }
}

void MultiInnovGenome::init_phenotype(Network &net)
{
  throw std::invalid_argument("init_phenotype not supported like this");
}
