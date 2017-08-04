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

#ifndef CPP_NEAT_ACCNEAT_SRC_UTIL_MAP_H_
#define CPP_NEAT_ACCNEAT_SRC_UTIL_MAP_H_

#pragma once

#include <string>
#include <map>

namespace NEAT
{

  struct Glyph
  {
    std::string type;
    char character;
    std::map<std::string, std::string> attrs;

    std::string
    str();
  };

  struct Location
  {
    struct
    {
      std::string row;
      std::string col;
    } label;
    struct
    {
      size_t row;
      size_t col;
    } index;

    bool operator<(const Location &other) const
    {
      if (index.row < other.index.row)
      {
        return true;
      } else if (index.row == other.index.row)
      {
        return index.col < other.index.col;
      } else
      {
        return false;
      }
    }
  };

  struct LocationTranslator
  {
    std::map<std::string, size_t> row_index;
    std::map<std::string, size_t> col_index;
    std::map<size_t, std::string> col_label;
    std::map<size_t, std::string> row_label;

    bool try_find(std::string row,
                  std::string col,
                  Location &result);
  };

  struct Object
  {
    Location loc;
    Glyph glyph;
    std::map<std::string, std::string> attrs;
  };

  struct Map
  {
    LocationTranslator loc_trans;
    size_t width;
    size_t height;
    std::map<Location, Object> objects;
  };

  Map parse_map(std::string path);

}

#endif
