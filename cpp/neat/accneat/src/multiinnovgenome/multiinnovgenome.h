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

#ifndef MULTIINNOVGENOME_H
#define MULTIINNOVGENOME_H

#include <list>
#include <memory>
#include <vector>

#include "genome.h"
#include "innovgenome/innovgenome.h"

namespace NEAT
{
  class MultiInnovGenome
          : public Genome
  {
    public:
    MultiInnovGenome(std::list< std::unique_ptr< InnovGenome>> &genome_list);

    virtual ~MultiInnovGenome() override;

    virtual Genome &operator=(const Genome &other) override;

    virtual void init_phenotype(class Network &net) override;

    virtual void print(std::ostream &out) const override;

    virtual void verify() override;

    virtual Stats get_stats() override;

    private:
    std::vector< std::unique_ptr< InnovGenome >> *innov_genome_list;
  };
}

#endif  //  MULTIINNOVGENOME_H
