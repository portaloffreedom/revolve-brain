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

#include <string>
#include <unistd.h>

#include "resource.h"
#include "util.h"

// Must be included first. Precompiled header with standard library includes.
#include "std.h"

//using namespace std;

namespace NEAT
{
  std::string find_resource(const std::string &name)
  {
    char home[1024];
    ssize_t rc = readlink("/proc/self/exe", home, sizeof(home));
    if (rc < 1)
    {
      error("Couldn't resolve /proc/self/exe! Is this Linux?");
    }
    if (rc == sizeof(home))
    {
      error("Possible buffer overrun.");
    }

    *strrchr(home, '/') = 0;

    return std::string(home) + "/res/" + name;
  }
}
