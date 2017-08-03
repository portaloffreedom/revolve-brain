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

#ifndef REVOLVE_NEAT_TEST_MULTIANNSPECIESNEAT_H_
#define REVOLVE_NEAT_TEST_MULTIANNSPECIESNEAT_H_

class TestMultiNNSpeciesNeat
{
  public:
  TestMultiNNSpeciesNeat();

  ~TestMultiNNSpeciesNeat();

  /// \brief Runs all tests. Returns false if one of the tests fails.
  bool  test();

  private:
  /// \brief test if the algorithm is able to resolve the XOR problem
  bool  testXOR();

  const int MAX_EVALUATIONS = 9999;
};

#endif  //  REVOLVE_NEAT_TEST_MULTIANNSPECIESNEAT_H_
