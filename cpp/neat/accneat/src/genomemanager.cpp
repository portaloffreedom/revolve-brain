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

#include "genomemanager.h"
#include "innovgenome/innovgenomemanager.h"
#include "util/util.h"

using namespace NEAT;

GenomeManager *
GenomeManager::create(const std::string &robot_name)
{
  switch (env->genome_type) {
    case GenomeType::INNOV:
      return new InnovGenomeManager(robot_name);
    default: panic();
  }
}
