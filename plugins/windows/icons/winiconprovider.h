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

#include <QCache>
#include <QString>
#include <QIcon>
#include <QMap>

#include <libdeskhare/shell/defaulticonprovider.h>

namespace Deskhare {

class WinIconProvider : public DefaultIconProvider
{
public:
  WinIconProvider();

  QIcon iconFromTheme(const QString& name) override;

  float getPriorityIndex() override;
  QString getDescription() const override;

private:
  QCache<QString, QIcon> cache_;
  QMap<QString, QStringList> icon_locations_;

  QIcon* resolve(const QString& name) const;
  void fillLocations();
};

} // namespace Deskhare
