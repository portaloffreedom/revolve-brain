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

#include <iostream>

#include "test_Actuator.h"

TestActuator::TestActuator(bool verbose)
        : verbose(verbose)
{
}

unsigned int TestActuator::outputs() const
{
  return 1;
}

void TestActuator::update(
        double *_output,
        double /*step*/)
{
  if (verbose)
  {
    std::cout << "TestActuator::update <- " << _output[0] << std::endl;
  }
}
