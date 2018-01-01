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
#include "evaluation/evaluationservice.h"
#include "evaluation/similarityevaluator.h"
#include "source.h"
#include "queryresultmodel.h"
#include "pluginmanager.h"
#include "history/historyservice.h"
#include "resultset.h"
#include "shell/iconprovider.h"
#include "utils/pluginutils.h"

namespace Deskhare {

static Q_LOGGING_CATEGORY(logger, "deskhare.controller")

Controller::Controller(QObject* parent)
: QObject(parent), history_service_(std::make_shared<HistoryService>())
{
  evaluation_service_registry_.registerEvaluator(history_service_);
  evaluation_service_registry_.registerEvaluator(
    std::make_shared<SimilarityEvaluator>());

  PluginContext plugincontext(&signals_, &sources_);
  plugin_manager_ = new PluginManager(plugincontext, this);

  IconService::setProvider(sources_.getBestFileIconProvider());

  result_model_ = new QueryResultModel(this);
  result_model_->setSources(sources_.getSources());
  qCDebug(logger) << sources_.getSources().length();

  actions_model_ = new QueryResultModel(this);
  actions_model_->setSources(sources_.getActionSources());
}

Controller::~Controller() = default;

void Controller::search(Query::Category category, const QString& query_string)
{
  auto query = std::make_shared<Query>(category, query_string);
  auto query_results = std::make_shared<ResultSet>(
    query,
    evaluation_service_registry_.getEvaluationService()
  );

  result_model_->setQuery(query, query_results);
  signals_.emitSearch(*query);
}

void Controller::searchAction(const Match& match, const QString& query_string)
{
  auto query = std::make_shared<Query>(Query::Category::All, query_string);
  auto query_results = std::make_shared<ResultSet>(
    query,
    evaluation_service_registry_.getEvaluationService()
  );

  actions_model_->setQuery(query, query_results);
  signals_.emitSearchAction(*query);
}

bool Controller::execute(const Match& match, const Action* action)
{
  history_service_->update(match.getUri());

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
