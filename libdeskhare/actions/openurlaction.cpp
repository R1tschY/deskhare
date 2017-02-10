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

#include <libdeskhare/actions/openurlaction.h>
#include <QDesktopServices>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(openUrlAction, "deskhare.openUrlAction")

namespace Deskhare {

OpenUrlAction::OpenUrlAction()
: Action()
{ }

QString OpenUrlAction::getDescription() const
{
  return tr("Do system standard operation.");
}

QString OpenUrlAction::getTitle() const
{
  return tr("Open");
}

QIcon OpenUrlAction::getIcon() const
{
  return QIcon();
}

bool OpenUrlAction::canHandleMatch(const Match& match) const
{
  // must be explicitly used as default action
  return false;
}

void OpenUrlAction::execute(const Match& match) const
{
  if (!QDesktopServices::openUrl(QUrl(match.getUri(), QUrl::TolerantMode)))
  {
    qCDebug(openUrlAction) << "Failed to open uri:" << match.getUri();
  }
}

} // namespace Deskhare
