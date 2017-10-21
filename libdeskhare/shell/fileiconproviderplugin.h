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

#include <QtPlugin>

#include "fileiconservice.h"

namespace Deskhare {

class FileIconProviderPlugin
{
public:
  virtual ~FileIconProviderPlugin() = default;

  /// \brief an index for the fitting of the icons to the system
  /// \retval 0 general plattform-independet implementation
  /// \retval 10 general implementation for the plattform (e.g. xdg)
  /// \retval 15 spezialized implementation for the plattform
  virtual float getFileIconProviderPriorityIndex() = 0;

  /// \brief get QFileIconProvider
  virtual std::unique_ptr<QFileIconProvider> getFileIconProvider() = 0;

  /// \brief description of FileIconProvider
  ///
  /// The contents to the first dot are used as title.
  virtual QString getFileIconProviderDescription() const = 0;
};

} // namespace Deskhare


