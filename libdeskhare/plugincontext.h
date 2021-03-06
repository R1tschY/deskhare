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

#include <tuple>
#include <utility>

namespace Deskhare {

class Signals;
class Registry;

class PluginContext
{
public:
  using Classes = std::tuple<const Signals*, Registry*>;

  template<typename...Args>
  PluginContext(Args&&...classes)
  : classes_(std::forward<Args>(classes)...)
  { }

  const Signals* getSignals() const
  { return std::get<const Signals*>(classes_); }

  Registry& getRegistry() const
  { return *std::get<Registry*>(classes_); }

  template<typename T>
  const T* get() const
  {
    return std::get<const T*>(classes_);
  }

private:
  Classes classes_;
};

} // namespace Deskhare


