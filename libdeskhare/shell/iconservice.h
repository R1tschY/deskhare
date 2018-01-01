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
#include <vector>

class QString;
class QIcon;
class QFileInfo;
class QFileIconProvider;

namespace Deskhare {

class IconProvider;

class IconService
{
public:
  static QIcon fileIcon(const QFileInfo &info);
  static QIcon iconFromTheme(const QString& name);

  static void setProvider(const std::shared_ptr<IconProvider>& provider);
  static std::shared_ptr<IconProvider> getProvider()
  { return getInstance().provider_; }

private:
  IconService();

  static IconService& getInstance();

  std::shared_ptr<IconProvider> provider_;
};

} // namespace Deskhare
