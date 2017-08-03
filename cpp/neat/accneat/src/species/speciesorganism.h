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

#include "organism.h"

namespace NEAT {

class SpeciesOrganism
        : public Organism
{
public:
    class Species *species;  //The Organism's Species
    real_t adjusted_fitness;
    real_t expected_offspring; //Number of children this Organism may have
    bool eliminate;  //Marker for destruction of inferior Organisms
    bool champion; //Marks the species champ
    int super_champ_offspring;  //Number of reserved offspring for a population leader

    SpeciesOrganism(const SpeciesOrganism &other);

    SpeciesOrganism(const Genome &genome);

    virtual ~SpeciesOrganism();

    virtual void
    init(int gen) override;

protected:
    virtual void
    copy_into(Organism &dst) const override;
};

}
