//
// deskhare
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

#include "systemcommandssource.h"

#include <QRegularExpression>
#include <libdeskhare/query.h>
#include <libdeskhare/source.h>
#include <libdeskhare/resultset.h>

#include "systemcommands.h"

namespace Deskhare {

SystemCommandsSource::SystemCommandsSource(const PluginContext& ctx)
: ctx_(ctx), commands_(getSystemCommands())
{ }

SystemCommandsSource::~SystemCommandsSource() = default;

bool SystemCommandsSource::canHandleQuery(const Query& query)
{
  return query.hasCategory(Query::Category::Action) &&
    !query.getSearchString().isEmpty();
}

void SystemCommandsSource::search(const Query& query,
  ResultSet& results)
{
  auto regex = query.getSearchRegex();

  std::vector<std::shared_ptr<Match>> matches;
  for (auto& command : commands_)
  {
    if (regex.match(command->getTitle()).hasMatch())
    {
      matches.emplace_back(command);
    }
  }
  results.sendMatches(matches);
}

QString SystemCommandsSource::getDescription() const
{
  return QString(
    "System commands.\n"
    "\n"
    "Execute commands to the system and the user session such as:\n"
    "\n"
    " * Shutdown\n"
    " * Reboot\n"
    " * Suspend\n"
    " * Hibernate\n"
    " * Logout\n"
    " * Lock\n"
  );
}

} // namespace Deskhare
