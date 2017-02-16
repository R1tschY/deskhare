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

#include "queriesexecutor.h"

#include <QFutureWatcher>
#include <QAbstractListModel>
#include <QtConcurrent>
#include <cpp-utils/scope.h>

#include "action.h"
#include "source.h"
#include "resultset.h"
#include "query.h"

namespace Deskhare {

class ResultSetModel : public MatchesModel
{
public:
  struct Entry
  {
    Entry() = default;
    Entry(std::unique_ptr<Match> match, std::unique_ptr<Action> defaultAction)
    : match(std::move(match)), defaultAction(std::move(defaultAction))
    { }

    std::unique_ptr<Match> match;
    std::unique_ptr<Action> defaultAction;
  };

  ResultSetModel(QObject* parent)
  : MatchesModel(parent)
  { }

  int rowCount(const QModelIndex& parent) const override;
  QVariant data(const QModelIndex& index, int role) const override;

  void takeMatches(std::vector<Entry>& entries);
  void clear();

  Match* getMatch(std::size_t row) override;

private:
  std::vector<Entry> entries_;
};

int ResultSetModel::rowCount(const QModelIndex & parent = QModelIndex()) const
{
  return entries_.size();
}

QVariant ResultSetModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid() || index.row() >= entries_.size())
    return QVariant();

  if (role == Qt::DisplayRole)
  {
#ifndef NDEBUG
    return entries_[index.row()].match->getTitle()
      + " (" + QString::number(entries_[index.row()].match->getScore()) + ")";
#else
    return entries_[index.row()].match->getTitle();
#endif
  }

  if (role == Qt::ToolTipRole)
  {
    return entries_[index.row()].match->getDescription();
  }

  if (role == Qt::DecorationRole)
  {
    return entries_[index.row()].match->getIcon();
  }

  if (role == QueriesExecutor::ScoreRole)
  {
    return entries_[index.row()].match->getScore();
  }

  if (role == QueriesExecutor::UriRole)
  {
    return entries_[index.row()].match->getUri();
  }

  return QVariant();
}

void ResultSetModel::takeMatches(std::vector<Entry>& entries)
{
  beginInsertRows(QModelIndex(),
    entries_.size(),
    entries_.size() + entries.size() - 1);
  scope(exit) { endInsertRows(); };

  entries_.insert(
    entries_.end(),
    std::make_move_iterator(entries.begin()),
    std::make_move_iterator(entries.end())
  );
  entries.clear();
}

void ResultSetModel::clear()
{
  beginRemoveRows(QModelIndex(), 0, entries_.size() - 1);
  scope(exit) { endRemoveRows(); };

  entries_.clear();
}

Match* ResultSetModel::getMatch(std::size_t row)
{
  if (row >= entries_.size())
    return nullptr;

  return entries_[row].match.get();
}

QueriesExecutor::QueriesExecutor()
{
  model_ = new ResultSetModel(this);
  future_watcher_ = new QFutureWatcher<Source*>(this);

  connect(
    future_watcher_, &QFutureWatcher<Source*>::finished,
    this, &QueriesExecutor::queryFinished);
}

QueriesExecutor::~QueriesExecutor() = default;

MatchesModel* QueriesExecutor::getModel()
{
  return model_;
}

void QueriesExecutor::setSources(const QVector<Source*>& sources)
{
  sources_ = sources;
}

class SourceSearcher
{
public:
  using result_type = Source*;

  SourceSearcher(const std::shared_ptr<const Query>& query,
    const std::shared_ptr<ResultSet>& results)
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

void QueriesExecutor::setQuery(
  Query::Categories categories,
  const QString& search_string)
{
  static_cast<ResultSetModel*>(model_)->clear();
  query_ = std::make_shared<Query>(categories, search_string);
  query_results_ = std::make_shared<ResultSet>(query_);

  future_watcher_->setFuture(
    QtConcurrent::mapped(sources_, SourceSearcher(query_, query_results_)));
}

void QueriesExecutor::queryFinished()
{
  std::vector<std::unique_ptr<Match>> matches;
  query_results_->recieveMatches(matches);
  std::sort(matches.begin(), matches.end(), MatchScoreComparer());

  std::vector<ResultSetModel::Entry> entries;
  for (auto& match : matches)
  {
    entries.emplace_back(std::move(match), nullptr);
  }

  model_->takeMatches(entries);
}

} // namespace Deskhare
