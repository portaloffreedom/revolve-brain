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

#ifndef REVOLVEBRAIN_BRAIN_PYTHON_LISTWRAP_H_
#define REVOLVEBRAIN_BRAIN_PYTHON_LISTWRAP_H_

#include <boost/python/object.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>

/// \brief boost::python::list is not iterable, so we create a wrapper to make
/// it iterable
template <typename ValueType>
class python_list_wrap
{
  private:
  const boost::python::list *wrapped_list;

  /// \brief cached list length value
  size_t list_length;

  public:
  python_list_wrap(const boost::python::list *list)
          : wrapped_list(list)
            , list_length(boost::python::len(*list))
  {}

  class iterator
          : public std::iterator<std::bidirectional_iterator_tag, ValueType>
  {
    private:
    const boost::python::list *wrapped_list;
    size_t index;

    public:
    iterator(const boost::python::list *list,
             const size_t index)
            : wrapped_list(list)
              , index(index)
    {}

    ValueType operator*() const
    {
      boost::python::object pyObject = (*wrapped_list)[index];
      return boost::python::extract<ValueType>(pyObject);
    }

    bool operator==(iterator const &rhs) const
    {
      return (this->wrapped_list == rhs.wrapped_list)
             and (this->index == rhs.index);
    }

    bool operatornot_eq(iterator const &rhs) const
    {
      return (this->wrapped_list not_eq rhs.wrapped_list)
             or (this->index not_eq rhs.index);
    }

    void operator++()
    {
      this->index++;
    }

    void operator--()
    {
      this->index--;
    }
  };

  iterator begin() const
  {
    return iterator(wrapped_list, 0);
  }

  iterator end() const
  {
    return iterator(wrapped_list, list_length);
  }

  size_t size() const
  {
    return boost::python::len(*wrapped_list);
  }
};

#endif  //  REVOLVEBRAIN_BRAIN_PYTHON_LISTWRAP_H_
