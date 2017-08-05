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

#include <string>
#include <vector>

#include "SUPGNeuron.h"
#include "innovgenome/innovgenome.h"

#include "SUPGGenomeManager.h"

SUPGGenomeManager::SUPGGenomeManager(const std::string &robot_name)
  : InnovGenomeManager(robot_name)
{
}

static NEAT::InnovGenome *to_innov(NEAT::Genome &g)
{
  return dynamic_cast<NEAT::InnovGenome *>(&g);
}

std::vector<std::unique_ptr<NEAT::Genome>>
SUPGGenomeManager::create_seed_generation(size_t ngenomes,
                                          NEAT::rng_t rng,
                                          size_t ntraits,
                                          size_t ninputs,
                                          size_t noutputs,
                                          size_t nhidden,
                                          const std::string &robot_name)
{
  NEAT::InnovGenome start_genome(rng,
                                 ntraits,
                                 ninputs,
                                 noutputs,
                                 nhidden,
                                 robot_name);

  const int node_id_bias = 1;
  const int node_id_input = node_id_bias + 1;
  const int node_id_output = node_id_input + ninputs;
//  const int node_id_hidden = node_id_output + noutputs;

  // ADD FIRST LINK ALREADY
  // for (int output_id = 0; output_id<noutputs-2; output_id++) {
  //     std::cout << "link: " << node_id_input + SUPGNeuron::TIMER
  //     << " → " << node_id_output + 2 + output_id << std::endl;
  start_genome.add_link(start_genome.links,
                        NEAT::InnovLinkGene(
                                rng.element(start_genome.traits).trait_id,
                                rng.prob(),
                                node_id_input + SUPGNeuron::TIMER,
                                node_id_output + 2,
                                false,
                                start_genome.get_last_gene_innovnum(),
                                0.0, robot_name, -1));
  //         }
  // FINISHED MODIFICATION

  std::vector<std::unique_ptr<NEAT::Genome>> genomes;
  {
    NEAT::rng_t _rng = rng;
    for (int i = 0; i < NEAT::env->pop_size; i++) {
      NEAT::InnovGenome *g = new NEAT::InnovGenome(robot_name);
      start_genome.duplicate_into(g);
      g->rng.seed(_rng.integer());
      g->mutate_link_weights(1.0, 1.0, NEAT::COLDGAUSSIAN);
      g->randomize_traits();

      genomes.emplace_back(std::unique_ptr<NEAT::Genome>(g));
    }
  }

  {
    NEAT::InnovGenome *g = to_innov(*genomes.back());

    // Keep a record of the innovation and node number we are on
    innovations.init(g->get_last_node_id(), g->get_last_gene_innovnum());
  }

  return genomes;
}
