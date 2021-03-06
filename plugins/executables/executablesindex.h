//
// deskhare
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

#include <vector>
#include <memory>
#include <QDateTime>
#include <QCoreApplication>

#include <libdeskhare/action.h>
#include <libdeskhare/indexes/sqliteindex.h>
#include <libdeskhare/indexes/indextimetable.h>

class QFileInfo;
class QString;

namespace Deskhare {

class IconService;


class ExecutableMatch : public Action
{
  Q_DECLARE_TR_FUNCTIONS(ExecutableMatch)
public:
  ExecutableMatch(
    const QString& commandLine, const QString& exePath,
    const QIcon& icon, float score);

  bool canHandleMatch(const Match& match) const override;
  void execute(const Match& target) const override;

private:
  QString exePath_;
};


class ExecutablesIndex : public SqliteIndex
{
public:
  ExecutablesIndex();

  bool addFile(const QFileInfo& file);

  void clear();

  std::vector<std::shared_ptr<Match>> search(const QString& query);

  QDateTime getLastIndexing()
  { return index_time_table_.getLastIndexing(); }

  void setLastIndexing(const QDateTime& time)
  { index_time_table_.setLastIndexing(time); }

private:
  bool create() override;
  bool upgrade(int currentFormatVersion) override;

  IndexTimeTable index_time_table_;
};

} // namespace Deskhare
