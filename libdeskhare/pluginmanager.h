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

#include <vector>
#include <QObject>
#include <QString>

#include "plugincontext.h"

namespace Deskhare {

class SourcePlugin;
class FileIconProviderPlugin;

/// \brief
class PluginManager : public QObject
{
  Q_OBJECT
public:
  PluginManager(const PluginContext& ctx, QObject* parent = 0);

  void loadPlugins();

  std::vector<QString> getSearchPaths() const;
  std::vector<SourcePlugin*> getSources() const;
  std::vector<FileIconProviderPlugin*> getFileIconProviders() const;
  const PluginContext& getContext() const { return ctx_; }

private:
  std::vector<QObject*> plugins_;
  std::vector<QString> plugin_paths_;
  PluginContext ctx_;

  void loadPlugin(const QString& filePath);
};

} // namespace Deskhare
