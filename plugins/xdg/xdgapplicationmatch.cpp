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
// TODO: use id: https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html#desktop-file-id
: Match("application://" + QFileInfo(desktopFile.fileName()).baseName(), score),
  desktopFile_(desktopFile)
{
  description_ = desktopFile_.localizedValue("Comment").toString();
  title_ = desktopFile_.localizedValue("Name").toString();

  icon_ = QIcon::fromTheme(desktopFile_.iconName());
  if (icon_.isNull())
    icon_ = QIcon::fromTheme(QStringLiteral("application-x-executable"));
}

QString XdgApplicationMatch::getDescription() const
{
  return description_;
}

QString XdgApplicationMatch::getTitle() const
{
  return title_;
}

QIcon XdgApplicationMatch::getIcon() const
{
  return icon_;
}

std::unique_ptr<Action> XdgApplicationMatch::getDefaultAction() const
{
  return std::make_unique<XdgApplicationAction>(
    desktopFile_, MatchScore::Highest);
}

} // namespace Deskhare
