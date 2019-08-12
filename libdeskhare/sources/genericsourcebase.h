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

#include <memory>
#include <QStringList>
#include <QVector>

#include "../source.h"
#include "../query.h"
#include "../match.h"

class QString;
class QIcon;

namespace Deskhare {

class Action;
class ResultSet;

/// \brief
class GenericSourceBase : public Source
{
public:
  bool canHandleQuery(const Query& query) override;
  void search(const Query& query, ResultSet& results) override;

  void clearMatches();
  void addMatch(const std::shared_ptr<Match>& match);

private:
  Query::Category categories_;
  QVector<std::shared_ptr<Match>> matches_;
};

} // namespace Deskhare
