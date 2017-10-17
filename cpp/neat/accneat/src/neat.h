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

#ifndef _NERO_NEAT_H_
#define _NERO_NEAT_H_

#include <cmath>
#include <string>
#include <vector>

#include "neattypes.h"

namespace NEAT
{
  enum functype
  {
    FT_SIGMOID = 0
  };

  enum mutator
  {
    GAUSSIAN = 0,
    COLDGAUSSIAN = 1
  };

  enum class GeneticSearchType
  {
    PHASED, BLENDED, COMPLEXIFY
  };

  enum class PopulationType
  {
    SPECIES,
    MULTI_NN_SPECIES
  };

  enum class GenomeType
  {
    INNOV
  };

  extern const std::vector< nodetype > nodetypes;

  inline bool is_input(nodetype type)
  {
    return (type == NT_BIAS) or (type == NT_SENSOR);
  }

  struct NeatEnv
  {
    GeneticSearchType search_type = GeneticSearchType::PHASED;
    PopulationType population_type = PopulationType::SPECIES;
    GenomeType genome_type = GenomeType::INNOV;

    class GenomeManager *genome_manager = nullptr;

    real_t trait_param_mut_prob = 0.5;

    /// \brief Power of mutation on a signle trait param
    real_t trait_mutation_power = 1.0;

    /// \brief Amount that mutation_num changes for a trait change inside a link
    real_t linktrait_mut_sig = 1.0;

    /// \brief Amount a mutation_num changes on a link connecting a node that
    /// changed its trait
    real_t nodetrait_mut_sig = 0.5;

    /// \brief The power of a linkweight mutation
    real_t weight_mut_power = 1.8;

    /// \brief Prob. that a link mutation which doesn't have to be recurrent
    /// will be made recurrent
    real_t recur_prob = 0.05;

    /// \brief These 3 global coefficients are used to determine the formula for
    /// computating the compatibility between 2 genomes.  The formula is:
    /// disjoint_coeff*pdg+excess_coeff*peg+mutdiff_coeff*mdmg.
    /// See the compatibility method in the Genome class for more info
    /// They can be thought of as the importance of disjoint Genes,
    /// excess Genes, and parametric difference between Genes of the
    /// same function, respectively.
    real_t disjoint_coeff = 1.0;
    real_t excess_coeff = 1.0;
    real_t mutdiff_coeff = 3.0;

    /// \brief This global tells compatibility threshold under which two Genomes
    /// are considered the same species
    real_t compat_threshold = 10.0;

    /// \brief How much does age matter?
    real_t age_significance = 1.0;

    /// \brief Percent of ave fitness for survival
    real_t survival_thresh = 0.4;

    /// \brief Prob. of a non-mating reproduction
    real_t mutate_only_prob = 0.25;

    real_t mutate_random_trait_prob = 0.1;
    real_t mutate_link_trait_prob = 0.1;
    real_t mutate_node_trait_prob = 0.1;
    real_t mutate_link_weights_prob = 0.8;
    real_t mutate_toggle_enable_prob = 0.1;
    real_t mutate_gene_reenable_prob = 0.05;
    real_t mutate_add_node_prob = 0.01;
    real_t mutate_delete_node_prob = 0.01;
    real_t mutate_add_link_prob = 0.3;
    real_t mutate_delete_link_prob = 0.3;
    bool mutate_add_link_reenables = false;

    /// \brief Prob. of a mate being outside species
    real_t interspecies_mate_rate =
            0.001;

    /// \brief
    real_t mate_multipoint_prob = 0.6;

    /// \brief Prob. of mating without mutation
    real_t mate_only_prob = 0.2;

    /// \brief Probability of forcing selection of ONLY links that are naturally
    /// recurrent
    real_t recur_only_prob = 0.2;

    /// \brief Size of population
    int pop_size = 1000;

    /// \brief Age where Species starts to be penalized
    int dropoff_age = 15;

    /// \brief Number of tries mutate_add_link will attempt to find an open link
    int newlink_tries = 20;

    /// \brief Tells to print population to file every n generations
    int print_every = 1000;

    /// \brief
    size_t num_runs = 1;
  };

  extern NeatEnv *env;

  // Random number generator; can pass seed value as argument
  // extern MRandomR250 NEATRandGen;

  /// const char *getUnit(const char *string, int index, const char *set);
  /// const char *getUnits(const char *string,
  ///                      int startIndex,
  ///                      int endIndex,
  ///                      const char *set);
  int getUnitCount(
          const char *string,
          const char *set);

  /// \brief SIGMOID FUNCTION ********************************
  /// This is a signmoidal activation function, which is an S-shaped squashing
  /// function
  /// It smoothly limits the amplitude of the output of a neuron to between 0
  /// and 1
  /// It is a helper to the neural-activation function get_active_out
  /// It is made inline so it can execute quickly since it is at every
  /// non-sensor node in a network.
  /// NOTE:  In order to make node insertion in the middle of a link possible,
  /// the signmoid can be shifted to the right and more steeply sloped:
  /// slope=4.924273
  /// constant= 2.4621365
  /// These parameters optimize mean squared error between the old output,
  /// and an output of a node inserted in the middle of a link between
  /// the old output and some other node.
  /// When not right-shifted, the steepened slope is closest to a linear
  /// ascent as possible between -0.5 and 0.5
  inline real_t fsigmoid(
          real_t activesum,
          real_t slope)
  {
    // NON-SHIFTED STEEPENED
    return (1 / (1 + (std::exp(-(slope * activesum)))));  // Compressed
  }

  /// \brief Hebbian Adaptation Function
  /// Based on equations in Floreano & Urzelai 2000
  /// Takes the current weight, the maximum weight in the containing network,
  /// the activation coming in and out of the synapse,
  /// and three learning rates for hebbian, presynaptic, and postsynaptic
  /// modification
  /// Returns the new modified weight
  /// NOTE: For an inhibatory connection, it makes sense to
  ///      emphasize decorrelation on hebbian learning!
  extern real_t oldhebbian(
          real_t weight,
          real_t maxweight,
          real_t active_in,
          real_t active_out,
          real_t hebb_rate,
          real_t pre_rate,
          real_t post_rate);

  /// \brief Hebbian Adaptation Function
  /// Based on equations in Floreano & Urzelai 2000
  /// Takes the current weight, the maximum weight in the containing network,
  /// the activation coming in and out of the synapse,
  /// and three learning rates for hebbian, presynaptic, and postsynaptic
  /// modification
  /// Returns the new modified weight
  /// NOTE: For an inhibatory connection, it makes sense to
  ///      emphasize decorrelation on hebbian learning!
  extern real_t hebbian(
          real_t weight,
          real_t maxweight,
          real_t active_in,
          real_t active_out,
          real_t hebb_rate,
          real_t pre_rate,
          real_t post_rate);

}  // namespace NEAT

#endif
