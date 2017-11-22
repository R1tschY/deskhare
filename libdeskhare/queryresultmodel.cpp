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

#include "queryresultmodel.h"

#include <QFutureWatcher>
#include <QAbstractListModel>
#include <QtConcurrent>
#include <cpp-utils/scope.h>

#include "action.h"
#include "source.h"
#include "resultset.h"
#include "query.h"

namespace Deskhare {

namespace {

class SourceSearcher
{
public:
  using result_type = int;

  SourceSearcher(
    const std::shared_ptr<const Query>& query,
    const std::shared_ptr<ResultSet>& results
  )
  : query_(query), results_(results)
  { }

  int operator()(std::shared_ptr<Source> source)
  {
    if (source->canHandleQuery(*query_))
    {
      source->search(*query_, *results_);
    }

    return 0;
  }

private:
  std::shared_ptr<ResultSet> results_;
  std::shared_ptr<const Query> query_;
};

} // namespace

QueryResultModel::QueryResultModel(QObject* parent)
: QAbstractListModel(parent)
{
  future_watcher_ = new QFutureWatcher<int>(this);

  connect(
    future_watcher_, &QFutureWatcher<Source*>::finished,
    this, &QueryResultModel::queryFinished);
}

QueryResultModel::~QueryResultModel() = default;

int QueryResultModel::rowCount(const QModelIndex & parent = QModelIndex()) const
{
  return entries_.size();
}

QVariant QueryResultModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid() || index.row() >= entries_.size())
    return QVariant();

  if (role == Qt::DisplayRole)
  {
#ifndef NDEBUG
    return QString(entries_[index.row()]->getTitle()
      + " (" + QString::number(entries_[index.row()]->getScore()) + ")");
#else
    return entries_[index.row()]->getTitle();
#endif
  }

  if (role == Qt::ToolTipRole)
  {
    return entries_[index.row()]->getDescription();
  }

  if (role == Qt::DecorationRole)
  {
    return entries_[index.row()]->getIcon();
  }

  if (role == QueryResultModel::ScoreRole)
  {
    return entries_[index.row()]->getScore();
  }

  if (role == QueryResultModel::UriRole)
  {
    return entries_[index.row()]->getUri();
  }

  return QVariant();
}

void QueryResultModel::takeMatches(std::vector<std::shared_ptr<Match>>& entries)
{
  if (entries.size() == 0)
    return;

  beginInsertRows(QModelIndex(),
    entries_.size(),
    entries_.size() + entries.size() - 1);
  auto guard = cpp::finally([&](){ endInsertRows(); });

  entries_.insert(
    entries_.end(),
    std::make_move_iterator(entries.begin()),
    std::make_move_iterator(entries.end())
  );
  entries.clear();
}

void QueryResultModel::clear()
{
  if (entries_.size() == 0)
    return;

  beginRemoveRows(QModelIndex(), 0, entries_.size() - 1);
  auto guard = cpp::finally([&](){ endRemoveRows();});

  entries_.clear();
}

Match* QueryResultModel::getMatch(std::size_t row)
{
  if (row >= entries_.size() || row < 0)
    return nullptr;

  return entries_[row].get();
}

void QueryResultModel::setSources(
  const QVector<std::shared_ptr<Source>>& sources)
{
  sources_ = sources;
}

void QueryResultModel::setQuery(
  const std::shared_ptr<Query>& query,
  const std::shared_ptr<ResultSet>& result_set)
{
  clear();

  QVector<std::shared_ptr<Source>> sourcesptrs;
  for (auto& source : sources_)
  {
    if (source->canHandleQuery(*query))
    {
      sourcesptrs.push_back(source);
    }
  }

  // TODO: really use threads everytime?
  QFuture<int> future =
    QtConcurrent::mapped(sourcesptrs, SourceSearcher(query, result_set));

  query_results_ = result_set;
  future_watcher_->setFuture(future);
}

void QueryResultModel::queryFinished()
{
  std::vector<std::shared_ptr<Match>> matches;
  query_results_->recieveMatches(matches);
  std::sort(matches.begin(), matches.end(), MatchScoreComparer());

  Matches entries;
  for (auto& match : matches)
  {
    entries.emplace_back(std::move(match));
  }

  takeMatches(entries);
}

} // namespace Deskhare
