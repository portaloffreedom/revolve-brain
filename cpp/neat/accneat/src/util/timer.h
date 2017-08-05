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

#ifndef CPP_NEAT_ACCNEAT_SRC_UTIL_TIMER_H_
#define CPP_NEAT_ACCNEAT_SRC_UTIL_TIMER_H_

#pragma once

#include <vector>

namespace NEAT
{
  class Timer
  {
    static std::vector<Timer *> timers;

    const char *_name;

    size_t _n = 0;

    double _total = 0.0;

    double _min = 0.0;

    double _max = 0.0;

    double _start = 0.0;

    double _recent = 0.0;

    public:
    Timer(const char *name);

    ~Timer();

    void start();

    void stop();

    static void report();
  };
}

#endif
