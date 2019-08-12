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
#include <tuple>

#include "filesystem/diriterator.h"
#include "shell/iconprovider.h"

namespace Deskhare {

static Q_LOGGING_CATEGORY(logger, "deskhare.pluginmanager")

PluginManager::Entry::~Entry() = default;

PluginManager::PluginManager(const PluginContext& ctx, QObject* parent)
: QObject(parent), ctx_(ctx)
{
  auto appDir = QCoreApplication::applicationDirPath();
  auto dataDirs = QStandardPaths::standardLocations(
    QStandardPaths::AppLocalDataLocation);

  plugin_paths_.push_back(appDir + "/plugins");
  foreach(QString dir, dataDirs)
  {
    if (dir == appDir)
      continue;

    plugin_paths_.push_back(dir + "/plugins");
  }

  loadPlugins();
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
      qCWarning(logger) << "skipped non-plugin" << filePath;
    return;
  }


  qCInfo(logger) << "loading plugin" << filePath;
  plugin_loaders_.emplace_back();

  auto& rawplugin = plugin_loaders_.back();
  rawplugin.loader.reset(new QPluginLoader(filePath));
  rawplugin.instance = rawplugin.loader->instance();
  if (!rawplugin.instance)
  {
    qCWarning(logger)
          << "cannot load plugin"
          << filePath
          << ":"
          << rawplugin.loader->errorString();
    return;
  }
  qCInfo(logger) << rawplugin.loader->metaData();

  auto* plugin = qobject_cast<Plugin*>(rawplugin.instance);
  if (plugin)
  {
    plugin->initialize(ctx_);
    plugins_.push_back(plugin);
    qCDebug(logger) << "loaded plugin" << filePath;
  }
  else
  {
    plugin_loaders_.pop_back();
    qCWarning(logger) << "non-plugin ignored:" << filePath;
  }
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
