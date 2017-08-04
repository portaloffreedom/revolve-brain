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
#ifndef _SPECIES_H_
#define _SPECIES_H_

#include "neat.h"
#include "multinnspeciesorganism.h"
#include "population.h"

namespace NEAT
{
  // ---------------------------------------------
  // SPECIES CLASS:
  //   A Species is a group of similar Organisms
  //   Reproduction takes place mostly within a
  //   single species, so that compatible organisms
  //   can mate.
  // ---------------------------------------------
  class MultiNNSpecies
  {
    public:
    /// \brief
    int id;

    /// \briefThe age of the Species
    int age;

    /// \brief The average fitness of the Species
    real_t ave_fitness;

    /// \brief Max fitness of the Species
    real_t max_fitness;

    /// \brief The max it ever had
    real_t max_fitness_ever;

    /// \brief
    int expected_offspring;

    /// \brief
    bool novel;

    /// \brief
    bool checked;

    /// \brief Allows killing off in competitive coevolution stagnation
    bool obliterate;

    /// \brief The organisms in the Species
    std::vector<MultiNNSpeciesOrganism *> organisms;

    /// \brief If this is too long ago, the Species will goes extinct
    int age_of_last_improvement;

    /// \brief When playing real-time allows estimating average fitness
    real_t average_est;

    /// \brief
    bool add_Organism(MultiNNSpeciesOrganism *o);

    /// \brief
    MultiNNSpeciesOrganism *first();

    //    bool
    //    print_to_file(std::ostream &outFile);

    /// \brief Change the fitness of all the organisms in the species to
    /// possibly depend slightly on the age of the species and then divide it by
    /// the size of the species so that the organisms in the species "share"
    /// the fitness
    void adjust_fitness();

    /// \brief
    real_t compute_average_fitness();

    /// \brief
    real_t compute_max_fitness();

    /// \brief Counts the number of offspring expected from all its members skim
    /// is for keeping track of remaining fractional parts of offspring and
    /// distributing them among species
    real_t count_offspring(real_t skim);

    /// \briefCompute generations since last improvement
    int last_improved()
    {
      return age - age_of_last_improvement;
    }

    /// \briefRemove an organism from Species
    void remove_eliminated();

    /// \brief
    void remove_generation(int gen);

    /// \brief
    real_t size()
    {
      return organisms.size();
    }

    /// \brief Perform mating and mutation to form next generation
    void reproduce(int ioffspring,
                   MultiNNSpeciesOrganism &baby,
                   class GenomeManager *genome_manager,
                   std::vector<MultiNNSpecies *> &sorted_species);

    // *** Real-time methods ***

    /// \brief
    MultiNNSpecies(int i);

    /// \brief Allows the creation of a Species that won't age (a novel one)
    /// This protects new Species from aging inside their first generation
    MultiNNSpecies(int i,
                   bool n);

    /// \brief
    ~MultiNNSpecies();
  };

  /// \brief This is used for list sorting of Species by fitness of best
  /// organism highest fitness first
  bool order_species(MultiNNSpecies *x,
                     MultiNNSpecies *y);

  /// \brief
  bool order_new_species(MultiNNSpecies *x,
                         MultiNNSpecies *y);

}

#endif
