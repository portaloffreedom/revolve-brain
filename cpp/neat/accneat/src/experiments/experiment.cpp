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

#include <map>
#include <string>
#include <vector>

#include "experiment.h"
#include "util/util.h"

using namespace NEAT;

////------------------------
///
/// CLASS Experiment
///
////------------------------
std::map< std::string, Experiment * > *Experiment::experiments = nullptr;

Experiment *
Experiment::get(const char *name)
{
  if (not experiments)
  {
    experiments = new std::map< std::string, Experiment * >();
  }
  auto it = experiments->find(name);
  return it == experiments->end() ? nullptr : it->second;
}

std::vector< std::string >
Experiment::get_names()
{
  std::vector< std::string > result;
  if (experiments)
  {
    for (auto &kv: *experiments)
    {
      result.push_back(kv.first);
    }
  }
  return result;
}

Experiment::Experiment(const char *name)
{
  this->name = name;
  if (get(name) not_eq nullptr)
  {
    trap("Experiment already registered: " << name);
  }
  experiments->insert(std::make_pair(name, this));
}

Experiment::~Experiment()
{
  experiments->erase(name);
  if (experiments->size() == 0)
  {
    delete experiments;
    experiments = nullptr;
  }
}
