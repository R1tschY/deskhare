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

#include "matchlistmodel.h"

#include <libdeskhare/match.h>

namespace Deskhare {

MatchListModel::MatchListModel(QObject *parent)
: QAbstractListModel(parent)
{
}

QVariant MatchListModel::data(const QModelIndex& index, int role) const
{
  auto row = static_cast<std::size_t>(index.row());
  if (!index.isValid() || row >= matches_.size())
      return QVariant();

  auto& match = matches_[row];
  Q_ASSERT(match);

  if (role == Qt::DisplayRole)
    return match->getTitle();

  if (role == Qt::DecorationRole)
    return match->getIcon();

  return QVariant();
}

Qt::ItemFlags MatchListModel::flags(const QModelIndex& index) const
{
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

int MatchListModel::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid())
      return 0;
  else
    return static_cast<int>(matches_.size());
}

void MatchListModel::addMatchs(std::vector<std::unique_ptr<Match>>&& match)
{
  beginInsertRows(QModelIndex(),
    matches_.size(), matches_.size() + match.size() -1);

  matches_.insert(matches_.end(),
    std::make_move_iterator(match.begin()),
    std::make_move_iterator(match.end()));

  endInsertRows();
}

Match* MatchListModel::getMatch(std::size_t row)
{
  if (row >= matches_.size())
    return nullptr;

  return matches_[row].get();
}

void MatchListModel::clear()
{
  beginRemoveRows(QModelIndex(), 0, matches_.size()-1);
  matches_.clear();
  endRemoveRows();
}

} // namespace Deskhare
