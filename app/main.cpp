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

#include <QApplication>
#include <QDebug>

#include <libdeskhare/utils/sharedlock.h>

#include "searchwindow.h"
#include "flatstyle.h"

namespace Deskhare {

void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  QByteArray localMsg = msg.toLocal8Bit();
  switch (type) {
  case QtDebugMsg:
      fprintf(stderr, "\x1b[37m[DEBUG] %s: %s\x1b[0m\n",
        context.category, localMsg.constData());
      fflush(stderr);
      break;
  case QtInfoMsg:
      fprintf(stderr, "\x1b[0m[INFO]\x1b[0m %s: %s\x1b[0m\n",
        context.category, localMsg.constData());
      fflush(stderr);
      break;
  case QtWarningMsg:
      fprintf(stderr, "\x1b[33m[WARN]\x1b[0m %s: %s\n",
        context.category, localMsg.constData());
      fflush(stderr);
      break;
  case QtCriticalMsg:
      fprintf(stderr, "\x1b[31m[CRIT]\x1b[0m %s: %s\x1b[0m\n",
        context.category, localMsg.constData());
      fflush(stderr);
      break;
  case QtFatalMsg:
      fprintf(stderr, "\x1b[31m[FATAL]\x1b[0m %s: %s\x1b[0m\n",
        context.category, localMsg.constData());
      abort();
  }
}

} // namespace Deskhare

using namespace Deskhare;

int main(int argc, char *argv[])
{
  qInstallMessageHandler(logHandler);
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
