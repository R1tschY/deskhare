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
#include <QDebug>
#include <QLoggingCategory>
#include <QtConcurrent>

#include "actions/runaction.h"
#include "query.h"
#include "sourceplugin.h"
#include "actionsourceplugin.h"
#include "evaluationservice.h"
#include "source.h"
#include "queryresultmodel.h"
#include "pluginmanager.h"
#include "shell/fileiconproviderplugin.h"
#include "history/historyservice.h"
#include "resultset.h"
#include "utils/pluginutils.h"

namespace Deskhare {

static Q_LOGGING_CATEGORY(logger, "deskhare.Controller")

namespace {

class SourceSearcher
{
public:
  using result_type = Source*;

  SourceSearcher(
    const std::shared_ptr<const Query>& query,
    const std::shared_ptr<ResultSet>& results
  )
  : query_(query), results_(results)
  { }

  Source* operator()(Source* source)
  {
    if (source->canHandleQuery(*query_))
    {
      source->search(*query_, *results_);
    }

    return source;
  }

private:
  std::shared_ptr<ResultSet> results_;
  std::shared_ptr<const Query> query_;
};

} // namespace


Controller::Controller(QObject* parent)
: QObject(parent), history_service_()
{
  evaluation_service_ = std::make_shared<EvaluationService>(history_service_);

  PluginContext plugincontext(&file_icon_provider_, &signals_);
  plugin_manager_ = new PluginManager(plugincontext, this);
  plugin_manager_->loadPlugins();

  file_icon_provider_.updateFromPlugins(
    plugin_manager_->getPlugins<FileIconProviderPlugin>());

  auto& ctx = plugin_manager_->getContext();
  for (auto* source : plugin_manager_->getPlugins<SourcePlugin>())
  {
    sources_.push_back(source->getSource(ctx));
    qCInfo(logger).noquote() << "got source:"
      << getTitleFromDescription(source->getSourceDescription());
  }
  QVector<Source*> sourcesptrs;
  for (auto& source : sources_)
  {
    sourcesptrs.push_back(source.get());
  }
  result_model_ = new QueryResultModel(this);
  result_model_->setSources(sourcesptrs);


  for (auto* source : plugin_manager_->getPlugins<ActionSourcePlugin>())
  {
    action_sources_.push_back(source->getActionSource(ctx));
  }
  QVector<Source*> actionsourcesptrs;
  for (auto& source : action_sources_)
  {
    actionsourcesptrs.push_back(source.get());
  }
  actions_model_ = new QueryResultModel(this);
  actions_model_->setSources(actionsourcesptrs);
}

Controller::~Controller() = default;

void Controller::search(Query::Category category, const QString& query_string)
{
  auto query = std::make_shared<Query>(category, query_string);
  auto query_results = std::make_shared<ResultSet>(query, evaluation_service_);

  QVector<Source*> sourcesptrs;
  for (auto& source : sources_)
  {
    if (source->canHandleQuery(*query))
    {
      sourcesptrs.push_back(source.get());
    }
  }
  auto future =
    QtConcurrent::mapped(sourcesptrs, SourceSearcher(query, query_results));

  result_model_->setQuery(query_results, future);
  signals_.emitSearch(*query);
}

void Controller::searchAction(const Match& match, const QString& query_string)
{
  auto query = std::make_shared<Query>(Query::Category::All, query_string);
  auto query_results = std::make_shared<ResultSet>(query, evaluation_service_);

  QVector<Source*> sourcesptrs;
  for (auto& source : action_sources_)
  {
    if (source->canHandleQuery(*query))
    {
      sourcesptrs.push_back(source.get());
    }
  }
  // TODO: really use threads?
  auto future =
    QtConcurrent::mapped(sourcesptrs, SourceSearcher(query, query_results));

  actions_model_->setQuery(query_results, future);
  signals_.emitSearchAction(*query);
}

bool Controller::execute(const Match& match, const Action* action)
{
  history_service_.update(match.getUri());

  std::shared_ptr<Action> _action;
  if (!action)
  {
    _action = match.getDefaultAction();
    action = _action.get();
    if (!action)
    {
      qCCritical(logger) << "match has no default action";
      return false;
    }
  }

  if (!action->canHandleMatch(match))
  {
    qCCritical(logger) << "action cannot execute match";
    return false;
  }

  action->execute(match);
  signals_.emitExecute(match, *action);

  return true;
}

} // namespace QuickStarter
