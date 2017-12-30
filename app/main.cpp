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

#include <windows.h>
#include <QApplication>
#include <QDebug>

#include <libdeskhare/utils/sharedlock.h>

#include "searchwindow.h"
#include "flatstyle.h"
#include "logging.h"

using namespace Deskhare;

int main(int argc, char *argv[], char *envp[])
{
  initLogging();

  qInfo() << "Starting deskhare ...";

  SharedLock lock("deskhare");
  if (!lock.hasLock())
  {
    qWarning() << "deskhare is already running";
    return 1;
  }

  QApplication a(argc, argv);
  a.setApplicationName(QLatin1String("deskhare"));
  a.setApplicationDisplayName(
    QCoreApplication::translate("Deskhare::App", "Deskhare"));
  a.setQuitOnLastWindowClosed(false);

  a.setStyle(new FlatStyle());

  //QIcon::setThemeName("gnome");
//  auto paths = QIcon::themeSearchPaths();
//  paths.append("$HOME/.icons");
//  QIcon::setThemeSearchPaths(paths);

  Deskhare::SearchWindow w;
  w.show();

  return a.exec();
}
