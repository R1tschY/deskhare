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

#include "xdgapplicationaction.h"

#include <QFileInfo>

#include "xdgcommon.h"

namespace Deskhare {

XdgApplicationAction::XdgApplicationAction(const XdgApplicationDesktopFile& app,
  float score)
: Action(
    tr("Execute"),
    tr("Execute application"),
    QIcon::fromTheme("system-run"),
    score
  ),
  app_(app)
{ }

XdgApplicationAction::XdgApplicationAction(const XdgApplicationDesktopFile& app,
  const QString& url, float score)
: Action(
    tr("Open with %1").arg(app.robustName()),
    tr("Opens selection with %1").arg(app.robustName()),
    createIcon(app.iconName()),
    score
  ),
  app_(app),
  url_(url)
{ }

bool XdgApplicationAction::canHandleMatch(const Match& match) const
{
  return true;
}

void XdgApplicationAction::execute(const Match& target) const
{
  bool success = app_.startDetached(url_);
  if (!success)
  {
    auto args = app_.expandExecString(QStringList(url_));
    qCCritical(xdgLogger) << "starting app failed:" << args;
  }
}

QIcon XdgApplicationAction::createIcon(const QString& iconName)
{
  QIcon icon = QIcon::fromTheme(iconName);
  if (icon.isNull())
    icon = QIcon::fromTheme(QLatin1String("application-x-executable"));
  return icon;
}

} // namespace Deskhare


