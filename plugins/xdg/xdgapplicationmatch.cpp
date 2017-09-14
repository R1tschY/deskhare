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

#include "xdgapplicationmatch.h"

#include <QFileInfo>
#include <libdeskhare/actions/runaction.h>

#include "xdgapplicationaction.h"

namespace Deskhare {

XdgApplicationMatch::XdgApplicationMatch(
  const XdgApplicationDesktopFile& desktopFile,
  float score)
: Match(
    desktopFile.localizedValue("Name").toString(),
    desktopFile.localizedValue("Comment").toString(),
    createIcon(desktopFile.iconName()),
    "xdg-app://" + desktopFile.id(),
    score
  ),
  desktopFile_(desktopFile)
{ }

std::shared_ptr<Action> XdgApplicationMatch::getDefaultAction() const
{
  return std::make_unique<XdgApplicationAction>(
    desktopFile_, MatchScore::Highest);
}

QIcon XdgApplicationMatch::createIcon(const QString& iconName)
{
  QIcon icon;

  if (!iconName.isEmpty())
    icon = QIcon::fromTheme(iconName);

  if (icon.isNull())
    icon = QIcon::fromTheme(QStringLiteral("application-x-executable"));

  return icon;
}

} // namespace Deskhare
