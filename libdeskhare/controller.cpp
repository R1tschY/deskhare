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

#include "../libdeskhare/controller.h"

#include <QString>
#include <QDebug>
#include <QPluginLoader>
#include <QVector>

#include "actions/runaction.h"
#include "query.h"
#include "sourceplugin.h"
#include "source.h"
#include "queryresultmodel.h"
#include "pluginmanager.h"

namespace Deskhare {

Controller::Controller(QObject* parent)
: QObject(parent),
  file_icon_provider_(),
  plugin_manager_(new PluginManager(PluginContext(&file_icon_provider_), this)),
  result_model_(new QueryResultModel(this)),
  actions_model_(new QueryResultModel(this))
{
  plugin_manager_->loadPlugins();

  file_icon_provider_.updateFromPlugins(plugin_manager_->getFileIconProviders());

  auto& ctx = plugin_manager_->getContext();
  for (auto* source : plugin_manager_->getSources())
  {
    sources_.push_back(source->getSource(ctx));
  }

  QVector<Source*> sourcesptrs;
  for (auto& source : sources_)
  {
    sourcesptrs.push_back(source.get());
  }

  result_model_->setSources(sourcesptrs);
}

Controller::~Controller() = default;

void Controller::search(const QString& query)
{
  result_model_->setQuery(Query::Categories::All, query);
}

void Controller::execute(const Match& match) const
{
  // TODO: add to history

  match.execute();
}

} // namespace QuickStarter
