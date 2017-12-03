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

#include <QString>
#include <QLoggingCategory>
#include <QSettings>
#include <QDir>
#include <QGuiApplication>

namespace QtAutostart {

namespace {

auto autorunPath = QLatin1String(
  "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run");

Q_LOGGING_CATEGORY(logger, "qtautostart")

void logSettingsError(QSettings& settings)
{
  settings.sync();

  switch (settings.status())
  {
  case QSettings::NoError:
    break;
  case QSettings::AccessError:
    qCCritical(logger) << "No access to write auto start setting";
    break;
  case QSettings::FormatError:
    qCCritical(logger) << "Wrong format for writing auto start setting";
    break;
  default:
    qCCritical(logger) << "Unknown error while writing auto start setting";
    break;
  }

}

} // namespace

bool addToAutostart(const QString& appName, const QString& commandLine)
{
  QString cmdLine = commandLine;
  if (cmdLine.isEmpty())
  {
    cmdLine = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
  }

  if (cmdLine.length() > 260)
  {
    qCCritical(logger)
      << "Too long (> 260 characters) command line:"
      << cmdLine;
    // TODO: use 6.2 format
    return false;
  }

  QSettings settings(autorunPath, QSettings::NativeFormat);
  settings.setValue(appName, cmdLine);

  logSettingsError(settings);
  return settings.status() == QSettings::NoError;
}

void removeFromAutostart(const QString& appName)
{
  QSettings settings(autorunPath, QSettings::NativeFormat);
  settings.remove(appName);
  logSettingsError(settings);
}

bool isInAutostart(const QString& appName)
{
  return QSettings(autorunPath, QSettings::NativeFormat).contains(appName);
}

} // namespace QtAutostart
