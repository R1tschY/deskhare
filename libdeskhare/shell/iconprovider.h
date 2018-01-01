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

#pragma once

#include <memory>
#include <QtPlugin>
#include <QFileIconProvider>

class QString;

namespace Deskhare {

class IconProvider
{
public:
  virtual ~IconProvider() = default;

  /// \brief an index for the fitting of the icons to the system
  /// \retval 0 general plattform-independet implementation
  /// \retval 10 general implementation for the platform (e.g. xdg)
  /// \retval 15 spezialized implementation for the platform
  virtual float getPriorityIndex() = 0;

  /// \brief get QFileIconProvider interface
  virtual const QFileIconProvider& getFileIconProvider() = 0;

  virtual QIcon iconFromTheme(const QString& name) = 0;

  /// \brief description of IconProviderPlugin
  ///
  /// The contents to the first dot are used as title.
  virtual QString getDescription() const = 0;
};

} // namespace Deskhare


