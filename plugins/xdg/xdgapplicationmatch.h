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

#pragma once

#include <libdeskhare/match.h>

#include <QString>
#include <QIcon>

#include "xdgapplicationdesktopfile.h"

namespace Deskhare {

/// \brief
class XdgApplicationMatch : public Match
{
public:
  XdgApplicationMatch(const XdgApplicationDesktopFile& desktopFile, float score);

  QString getDescription() const override;
  QString getTitle() const override;
  QIcon getIcon() const override;

  std::unique_ptr<Action> getDefaultAction() const override;

private:
  QString title_;
  QString description_;
  QIcon icon_;
  XdgApplicationDesktopFile desktopFile_;
};

} // namespace Deskhare
