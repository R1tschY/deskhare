//
// deskhare - cross-platform quick launcher
// Copyright (C) 2017 Richard Liebscher
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#pragma once

#include <QDirIterator>

namespace Deskhare {

struct DirIteratorSentinal { };

class DirIterator : public std::iterator<std::input_iterator_tag, const QDirIterator>
{
public:
  DirIterator(QDirIterator& it) : it_(&it), dirty_(true)
  { }

  DirIterator& operator++()
  {
    if (dirty_)
      it_->next();

    dirty_ = true;
    return *this;
  }

  const QDirIterator& operator*()
  {
    if (dirty_)
    {
      it_->next();
      dirty_ = false;
    }

    return *it_;
  }

  bool operator==(DirIteratorSentinal sentinal)
  {
    return !it_->hasNext();
  }

  bool operator!=(DirIteratorSentinal sentinal)
  {
    return it_->hasNext();
  }

private:
  QDirIterator* it_;
  bool dirty_;
};

class DirRange
{
public:
  template<typename...Args>
  DirRange(Args&&...args)
    : it_(std::forward<Args>(args)...)
  { }

  DirIterator begin()
  {
    return DirIterator(it_);
  }

  DirIteratorSentinal end()
  {
    return DirIteratorSentinal();
  }

private:
  QDirIterator it_;
};

} // namespace Deskhare


