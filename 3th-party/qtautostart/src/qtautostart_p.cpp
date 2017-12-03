//
// qtautostart
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

#include "qtautostart.h"

#include <QCoreApplication>
#include <QDir>
#include "qtautostart_p.h"

namespace QtAutostart {

namespace {

QString getAppName(const QString& appName)
{
  return appName.isEmpty() ? QCoreApplication::applicationName() : appName;
}

QString getCommandLine(const QString& commandLine)
{
  return commandLine.isEmpty()
    ? QDir::toNativeSeparators(QCoreApplication::applicationFilePath())
    : commandLine;
}

} // namespace

bool addToAutostart(const QString& appName, const QString& commandLine)
{
  return addToAutostart_private(
    getAppName(appName), getCommandLine(commandLine)
  );
}

void removeFromAutostart(const QString& appName)
{
  removeFromAutostart_private(getAppName(appName));
}

bool isInAutostart(const QString& appName)
{
  return isInAutostart_private(getAppName(appName));
}

} // namespace QtAutostart
