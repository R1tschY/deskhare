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

#include "historyservice.h"

#include <algorithm>
#include <QString>
#include <QDateTime>

#include "historyindex.h"
#include "../match.h"

namespace Deskhare {

HistoryService::HistoryService()
: index_(new HistoryIndex())
{
  if (!index_->open(SqliteIndex::createIndexPath("history.db")))
    return;
}

HistoryService::~HistoryService() = default;

void HistoryService::update(const QString& uri, const QDateTime& time)
{
  index_->update(uri, time.toMSecsSinceEpoch() / 1000);
}

void HistoryService::update(const QString& uri, time_t time)
{
  index_->update(uri, time);
}

float HistoryService::eval(const Query& query, const Match& match)
{
  auto stats = index_->getStats(match.getUri());
  auto delta_t = std::min(
    std::time_t(24 * 3600),
    std::time(nullptr) - std::get<0>(stats)
  );
  if (delta_t > 0)
    return std::min(1.5f, ((24.f * 3600.f) / delta_t) * std::get<1>(stats));
  else
    return 0.f;
}

} // namespace Deskhare
