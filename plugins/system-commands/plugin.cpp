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

#include "plugin.h"

#include <QDebug>
#include <QStandardPaths>
#include <QDirIterator>
#include <QSqlError>
#include <QSqlQuery>
#include <QLoggingCategory>
#include <QFile>
#include <QDateTime>

#include <libdeskhare/query.h>
#include <libdeskhare/source.h>
#include <libdeskhare/resultset.h>
#include <libdeskhare/registry.h>

#include "plugin.h"
#include "systemcommandssource.h"

namespace Deskhare {

void SystemCommandsPlugin::initialize(const PluginContext& ctx)
{
  auto& registry = ctx.getRegistry();
  registry.registerSource(std::make_shared<SystemCommandsSource>(ctx));
}

QString SystemCommandsPlugin::getDescription()
{
  return QString(
    "System commands plugin.\n"
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
