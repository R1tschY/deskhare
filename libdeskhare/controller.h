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

#ifndef APP_PLUGINS_QUICKSTARTER_CONTROLLER_H_
#define APP_PLUGINS_QUICKSTARTER_CONTROLLER_H_

#include <vector>
#include <memory>

#include "shell/fileiconservice.h"
#include "query.h"
#include "signals.h"
#include "history/historyservice.h"

class QString;
class QAbstractListModel;

namespace Deskhare {

class Source;
class Action;
class Match;
class QueryResultModel;
class PluginManager;
class EvaluationService;

class Controller : QObject
{
  Q_OBJECT
public:
  Controller(QObject* parent = 0);
  ~Controller();

  QueryResultModel* getQueryResultModel() { return result_model_; }
  QueryResultModel* getActionQueryResultModel() { return actions_model_; }

  bool execute(const Match& match, const Action* action = nullptr);

public slots:
  void search(Query::Category category, const QString& query);

private:
  std::vector<std::unique_ptr<Source>> sources_;
  std::vector<std::unique_ptr<Source>> action_sources_;
  FileIconProvider file_icon_provider_;
  HistoryService history_service_;
  std::shared_ptr<EvaluationService> evaluation_service_;
  Signals signals_;
  PluginManager* plugin_manager_ = nullptr;
  QueryResultModel* result_model_ = nullptr;
  QueryResultModel* actions_model_ = nullptr;
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_CONTROLLER_H_ */
