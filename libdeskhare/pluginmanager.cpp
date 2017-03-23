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

#include "pluginmanager.h"

#include <cpp-utils/algorithm/container.h>
#include <iterator>
#include <QCoreApplication>
#include <QDirIterator>
#include <QLibrary>
#include <QLoggingCategory>
#include <QPluginLoader>
#include <QStandardPaths>
#include <vector>

#include "filesystem/diriterator.h"
#include "sourceplugin.h"
#include "shell/fileiconproviderplugin.h"

namespace Deskhare {

Q_LOGGING_CATEGORY(pluginManager, "deskhare.pluginmanager")

PluginManager::PluginManager(const PluginContext& ctx, QObject* parent)
: QObject(parent), ctx_(ctx)
{
  auto appDir = QCoreApplication::applicationDirPath();
  auto dataDirs = QStandardPaths::standardLocations(
    QStandardPaths::AppLocalDataLocation);

  plugin_paths_.push_back(appDir + "/plugins");
  foreach(QString dir, dataDirs)
  {
    plugin_paths_.push_back(dir + "/plugins");
  }
}

std::vector<QString> PluginManager::getSearchPaths() const
{
  return plugin_paths_;
}

void PluginManager::loadPlugin(const QString& filePath)
{
  if (!QLibrary::isLibrary(filePath))
  {
    if (filePath.length() > 3)
      qCWarning(pluginManager) << "skipped non-plugin" << filePath;
    return;
  }

  qCInfo(pluginManager) << "loading plugin" << filePath;
  QPluginLoader pluginLoader(filePath);
  QObject* plugin = pluginLoader.instance();
  if (!plugin)
  {
    qCWarning(pluginManager)
          << "cannot load plugin"
          << filePath
          << ":"
          << pluginLoader.errorString();
    return;
  }

  plugin->setParent(this);
  plugins_.push_back(plugin);

  qCDebug(pluginManager) << "loaded plugin" << filePath;
}

void PluginManager::loadPlugins()
{
  for (const auto& dir : plugin_paths_)
  {
    cpp::for_each(DirRange(dir, QDir::Files | QDir::Readable),
    [&](const QDirIterator& it){
      loadPlugin(it.filePath());
    });
  }
}

} // namespace Deskhare

