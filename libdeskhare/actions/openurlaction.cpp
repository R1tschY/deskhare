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


namespace Deskhare {

static Q_LOGGING_CATEGORY(logger, "deskhare.openUrlAction")

OpenUrlAction::OpenUrlAction(float score)
: Action(
  tr("Open"),
  tr("Do system standard operation."),
  QIcon(),
  score)
{ }

bool OpenUrlAction::canHandleMatch(const Match& match) const
{
  return QUrl(match.getUri(), QUrl::TolerantMode).isValid();
}

void OpenUrlAction::execute(const Match& match) const
{
  qCDebug(logger) << "Open URL" << match.getUri();
  if (!QDesktopServices::openUrl(QUrl(match.getUri(), QUrl::TolerantMode)))
  {
    qCCritical(logger) << "Failed to open uri:" << match.getUri();
  }
}

} // namespace Deskhare
