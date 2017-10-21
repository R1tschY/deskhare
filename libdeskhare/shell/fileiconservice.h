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

#include <QFileIconProvider>
#include <memory>
#include <vector>

namespace Deskhare {

class FileIconProviderPlugin;

class FileIconProvider
{
public:
  FileIconProvider();

  QIcon icon(QFileIconProvider::IconType type) const;
  QIcon icon(const QFileInfo &info) const;
  QString type(const QFileInfo &info) const;

  void setProvider(std::unique_ptr<QFileIconProvider> provider);
  QFileIconProvider* getProvider() { return provider_.get(); }

  void setPlugin(const std::shared_ptr<FileIconProviderPlugin>& plugin);

private:
  std::unique_ptr<QFileIconProvider> provider_;
};

} // namespace Deskhare
