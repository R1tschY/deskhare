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
#include <memory>
#include <QObject>
#include <QString>

#include "plugincontext.h"
#include "plugin.h"

class QPluginLoader;

namespace Deskhare {

/// \brief
class PluginManager : public QObject
{
  Q_OBJECT
public:
  struct Entry
  {
    Entry() = default;
    Entry(Entry&&) = default;
    ~Entry();

    Entry& operator=(Entry&& __u) noexcept = default;

    std::unique_ptr<QPluginLoader> loader;
    QObject* instance = nullptr;
  };

  PluginManager(const PluginContext& ctx, QObject* parent = 0);

  template<typename Plugin>
  std::vector<Plugin*> getPlugins();

  const std::vector<Entry>& getTopLevelPlugins() const
  { return plugin_loaders_; }

  std::vector<QString> getSearchPaths() const;
  const PluginContext& getContext() const { return ctx_; }

private:
  std::vector<QString> plugin_paths_;
  std::vector<Entry> plugin_loaders_;
  std::vector<Plugin*> plugins_;

  PluginContext ctx_;

  void loadPlugins();
  void loadPlugin(const QString& filePath);
};

template<typename Plugin>
inline std::vector<Plugin*> PluginManager::getPlugins()
{
  std::vector<Plugin*> result;
  result.reserve(plugin_loaders_.size());

  for (const Entry& plugin : plugin_loaders_)
  {
    auto* source = qobject_cast<Plugin*>(plugin.instance);
    if (source)
      result.push_back(source);
  }
  return result;
}

} // namespace Deskhare
