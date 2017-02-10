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

namespace Deskhare {

namespace {

class ResultSetModel : public QAbstractListModel
{
public:
  struct Entry
  {
    std::unique_ptr<Match> match;
    std::unique_ptr<Action> defaultAction;
  };

  ResultSetModel(QObject* parent)
  : QAbstractListModel(parent)
  { }

  int rowCount(const QModelIndex& parent) const override;
  QVariant data(const QModelIndex& index, int role) const override;

  void takeMatches(std::vector<Entry>& entries);
  void clear();

private:
  std::vector<Entry> entries_;
};

int ResultSetModel::rowCount(const QModelIndex & parent = QModelIndex()) const
{
  return entries_.size();
}

QVariant ResultSetModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid() || index.row() > entries_.size())
    return QVariant();

  if (role == Qt::DisplayRole)
  {
    return entries_[index.row()].match->getTitle();
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

} // namespace


QueriesExecutor::QueriesExecutor()
{
  model_ = new ResultSetModel(this);
  future_watcher_ = new QFutureWatcher<void>(this);
}

QueriesExecutor::~QueriesExecutor() = default;

QAbstractListModel* QueriesExecutor::getModel()
{
  return model_;
}

void QueriesExecutor::setSources(std::vector<Source*> sources_)
{
  sources_ = std::move(sources_);
}

void QueriesExecutor::setQuery(const Query& query)
{
  static_cast<ResultSetModel*>(model_)->clear();

//  QtConcurrent::mapped(sources_,
}

} // namespace Deskhare
