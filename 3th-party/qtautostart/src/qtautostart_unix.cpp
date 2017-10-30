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

#include <QStandardPaths>
#include <QString>
#include <QLoggingCategory>
#include <QFileInfo>
#include <QDir>
#include <QGuiApplication>

namespace QtAutostart {

namespace {

QString getAutostartDesktopFilePath(const QString& appName)
{
  return
    QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
    + QLatin1String("/autostart/") + appName + QLatin1String(".desktop");
}

Q_LOGGING_CATEGORY(logger, "qtautostart")

} // namespace

bool addToAutostart(const QString& appName)
{
  QFileInfo autostartFilePath = getAutostartDesktopFilePath(
    appName.isEmpty() ? QCoreApplication::applicationName() : appName);

  if (!autostartFilePath.dir().mkpath("."))
  {
    qCWarning(logger) << "Could not create autostart folder";
    return false;
  }

  QFile fp(autostartFilePath.filePath());
  if (!fp.open(QIODevice::WriteOnly))
  {
    qCWarning(logger)
      << "Could not open autostart entry:"
      << fp.fileName()
      << "message ="
      << fp.errorString();
    return false;
  }

  qCDebug(logger) << "Writing" << fp.fileName() << "...";

  QTextStream ts(&fp);
  ts.setCodec("UTF-8");
  ts << QString(
    "[Desktop Entry]\n"
    "Name=%1\n"
    "Exec=%2\n"
    "Icon=%3\n"
    "Terminal=false\n"
    "Type=Application\n"
    "StartupNotify=false\n"
    "X-GNOME-Autostart-enabled=true\n"
  ).arg(
    QGuiApplication::applicationDisplayName(),
    QGuiApplication::applicationFilePath(),
    appName.isEmpty()
    ? QCoreApplication::applicationName().toLower()
    : appName.toLower()
  );

  if (ts.status())
  {
    qCWarning(logger)
      << "Could not write autostart entry:"
      << fp.fileName()
      << "message ="
      << fp.errorString();
    return false;
  }

  return true;
}

void removeFromAutostart(const QString& appName)
{
  if (!QFile::remove(getAutostartDesktopFilePath(
    appName.isEmpty() ? QCoreApplication::applicationName() : appName)))
  {
    qCWarning(logger) << "Could not remove autostart desktop file";
  }
}

bool isInAutostart(const QString& appName)
{
  return QFileInfo::exists(getAutostartDesktopFilePath(
    appName.isEmpty() ? QCoreApplication::applicationName() : appName));
}

} // namespace QtAutostart
