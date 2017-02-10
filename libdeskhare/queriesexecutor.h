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

class QAbstractListModel;
template<typename T> class QFutureWatcher;

namespace Deskhare {

class Match;
class Source;
class Query;

/// \brief
class QueriesExecutor : public QObject
{
  Q_OBJECT
public:
  enum {
    ScoreRole = Qt::UserRole, // int
    UriRole = Qt::UserRole + 1 // QString
  };


  QueriesExecutor();
  ~QueriesExecutor();

  QAbstractListModel* getModel();
  void setSources(std::vector<Source*> sources_);

public:
  void setQuery(const Query& query);

private:
  QAbstractListModel* model_;
  QFutureWatcher<void>* future_watcher_;
  std::vector<Source*> sources_;


};

} // namespace Deskhare
