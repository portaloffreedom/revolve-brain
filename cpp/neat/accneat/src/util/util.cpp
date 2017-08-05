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
#include <sys/stat.h>
#include <vector>

// Must be included first. Precompiled header with standard library includes.
#include "std.h"
#include "util.h"

void mkdir(const std::string &path)
{
  int status = ::mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  if (0 != status)
  {
    char buf[2048];
    std::sprintf(buf, "Failed making directory '%s'", path.c_str());
    std::perror(buf);
    exit(1);
  }
}

bool exists(const std::string &path)
{
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
}

std::vector<std::string> permute_repeat(const std::string &letters,
                                        size_t len)
{
  std::vector<std::string> result;
  std::string buf;

  struct local
  {
    static void __permute(const std::string &letters,
                          size_t depth,
                          size_t len,
                          std::vector<std::string> &result,
                          std::string &buf)
    {
      if (depth == len)
      {
        result.push_back(buf);
      }
      else
      {
        for (size_t i = 0; i < letters.size(); ++i)
        {
          buf.append(letters, i, 1);
          __permute(letters, depth + 1, len, result, buf);
          buf.erase(buf.size() - 1);
        }
      }
    }
  };

  local::__permute(letters, 0, len, result, buf);

  return result;
}
