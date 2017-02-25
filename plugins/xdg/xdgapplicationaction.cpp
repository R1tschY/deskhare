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
: Action(score), app_(app)
{
  description_ = tr("Execute application");
  title_ = tr("Execute");
  icon_ = QIcon::fromTheme("system-run");
}

XdgApplicationAction::XdgApplicationAction(const XdgApplicationDesktopFile& app,
  const QString& url, float score)
: Action(score), app_(app), url_(url)
{
  auto appName = app.robustName();

  description_ = tr("Opens selection with %1").arg(appName);
  title_ = tr("Open with %1").arg(appName);
  icon_ = QIcon::fromTheme(app.iconName());
  if (icon_.isNull())
    icon_ = QIcon::fromTheme(QStringLiteral("application-x-executable"));
}

QString XdgApplicationAction::getDescription() const
{
  return description_;
}

QString XdgApplicationAction::getTitle() const
{
  return title_;
}

QIcon XdgApplicationAction::getIcon() const
{
  return icon_;
}

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

} // namespace Deskhare


