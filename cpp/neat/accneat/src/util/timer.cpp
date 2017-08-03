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

#include "std.h" // Must be included first. Precompiled header with standard library includes.
#include "timer.h"
#include <assert.h>
#include <sys/time.h>

using namespace NEAT;
using namespace std;

vector<Timer *> Timer::timers;

static double
seconds()
{
  struct timeval tv;
  gettimeofday(&tv,
               NULL);

  return double(tv.tv_sec + tv.tv_usec / 1000000.0);
}

Timer::Timer(const char *name) :
        _name(name)
{
  timers.push_back(this);
}

Timer::~Timer()
{
  timers.erase(find(timers.begin(),
                    timers.end(),
                    this));
}

void
Timer::start()
{
  assert(_start == 0.0);

  _start = seconds();
}

void
Timer::stop()
{
  assert(_start != 0.0);

  double t = seconds() - _start;
  _recent = t;
  _start = 0.0;

  if (_n == 0) {
    _min = _max = t;
  } else {
    _min = min(_min,
               t);
    _max = max(_max,
               t);
  }
  _total += t;
  _n++;
}

void
Timer::report()
{
  for (Timer *t: timers) {
    cout
            << t->_name
            << ": n="
            << t->_n
            << ", recent="
            << t->_recent
            << ", mean="
            << (t->_total / t->_n)
            << ", min="
            << t->_min
            << ", max="
            << t->_max
            << ", total="
            << t->_total
            << endl;
  }
}
