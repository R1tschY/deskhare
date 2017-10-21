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

#ifndef APP_PLUGINS_QUICKSTARTER_SOURCES_SOURCE_H_
#define APP_PLUGINS_QUICKSTARTER_SOURCES_SOURCE_H_

#include <vector>
#include <memory>
#include <utility>
#include <tuple>

#include "match.h"
#include "plugincontext.h"

class QString;

namespace Deskhare {

class Query;
class Match;
class PluginContext;
class ResultSet;

/// \brief
class Source
{
public:
  virtual ~Source() = default;

  virtual bool canHandleQuery(const Query& query) = 0;
  virtual void search(const Query& query, ResultSet& results) = 0;

  virtual QString getDescription() const = 0;
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_SOURCES_SOURCE_H_ */
