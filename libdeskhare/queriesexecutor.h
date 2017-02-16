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

#include <QObject>
#include <vector>
#include <memory>
#include <QVector>
#include <QAbstractListModel>

#include "query.h"

class QAbstractListModel;
template<typename T> class QFutureWatcher;

namespace Deskhare {

class Match;
class Source;
class Query;
class ResultSet;
class ResultSetModel;

class MatchesModel : public QAbstractListModel
{
public:
  using QAbstractListModel::QAbstractListModel;

  virtual Match* getMatch(std::size_t row) = 0;
};

/// \brief
class QueriesExecutor : public QObject
{
  Q_OBJECT
public:
  enum {
    MatchRole = Qt::UserRole, // Match*
    ScoreRole, // int
    UriRole // QString
  };


  QueriesExecutor();
  ~QueriesExecutor();

  MatchesModel* getModel();
  void setSources(const QVector<Source*>& sources);

public:
  void setQuery(Query::Categories categories, const QString& search_string);

private:
  ResultSetModel* model_;
  QFutureWatcher<Source*>* future_watcher_;
  QVector<Source*> sources_;

  std::shared_ptr<ResultSet> query_results_;
  std::shared_ptr<const Query> query_;

  void queryFinished();
};

} // namespace Deskhare
