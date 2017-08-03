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

#ifndef REVOLVEBRAIN_BRAIN_PYTHON_ARRAY_H_
#define REVOLVEBRAIN_BRAIN_PYTHON_ARRAY_H_

template <typename ValueType>
class python_array
{
private:
    ValueType *const value_pointer;

public:
    python_array(ValueType *const array)
            :
            value_pointer(array)
    {}

    python_array(const python_array &array)
            :
            value_pointer(array.value_pointer)
    {}

    ValueType &
    operator[](const long index) const
    {
      return value_pointer[index];
    }

    ValueType
    getitem(const long key) const
    {
      return value_pointer[key];
    }

    void
    setitem(const long key,
            const ValueType value)
    {
      value_pointer[key] = value;
    }

};

#endif // REVOLVEBRAIN_BRAIN_PYTHON_ARRAY_H_
