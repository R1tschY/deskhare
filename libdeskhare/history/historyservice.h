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

#include <ctime>
#include <memory>

#include "../evaluator.h"

class QString;
class QDateTime;

namespace Deskhare {

class HistoryIndex;
class Query;
class Match;

/// \brief
class HistoryService : public Evaluator
{
public:
  HistoryService();
  ~HistoryService();

  void update(const QString& uri, const QDateTime& time);
  void update(const QString& uri, std::time_t time = std::time(nullptr));

  float eval(const Query& query, const Match& match) override;

private:
  std::unique_ptr<HistoryIndex> index_;
};

} // namespace Deskhare
