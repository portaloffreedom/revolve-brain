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

#ifndef REVOLVEBRAIN_BRAIN_PYTHON_EVALUATORWRAP_H_
#define REVOLVEBRAIN_BRAIN_PYTHON_EVALUATORWRAP_H_

#include <iostream>

#include <boost/python/wrapper.hpp>
#include <boost/python/object.hpp>

#include "brain/Evaluator.h"

namespace revolve
{
  namespace brain
  {
    /// \brief Class needed to enable calling virtual methods implemented child
    /// python classes
    class EvaluatorWrap
            : public Evaluator
              , public boost::python::wrapper<Evaluator>
    {
      public:
      virtual void start()
      {
        this->get_override("start")();
      }

      virtual double fitness()
      {
        return this->get_override("fitness")();
      };
    };
  }
}

#endif  //  REVOLVEBRAIN_BRAIN_PYTHON_EVALUATORWRAP_H_
