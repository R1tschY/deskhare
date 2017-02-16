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

#include <memory>
#include <vector>
#include <QAbstractListModel>

#include <libdeskhare/queriesexecutor.h>

namespace Deskhare {

class Match;

/// \brief
class MatchListModel : public MatchesModel
{
public:
  MatchListModel(QObject *parent = 0);

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;

  int rowCount(const QModelIndex &parent) const override;

  void addMatchs(std::vector<std::unique_ptr<Match>>&& match);
  Match* getMatch(std::size_t row) override;
  void clear();

private:
  std::vector<std::unique_ptr<Match>> matches_;
};

} // namespace Deskhare
