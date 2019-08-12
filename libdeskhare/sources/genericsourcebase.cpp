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

#include "genericsourcebase.h"

#include <vector>
#include <QRegularExpression>

#include "../resultset.h"

namespace Deskhare {

bool GenericSourceBase::canHandleQuery(const Query& query)
{
  return !query.getSearchString().isEmpty() && query.hasCategory(categories_);
}

void GenericSourceBase::search(const Query& query,
  ResultSet& results)
{
  auto regex = query.getSearchRegex();

  std::vector<std::shared_ptr<Match>> matches;
  for (auto& match : matches_)
  {
    auto keywords = match->getKeywords();
    if (keywords.isEmpty())
    {
      if (regex.match(match->getTitle()).hasMatch())
      {
        matches.emplace_back(match);
      }
    }
    else
    {
      for (auto& keyword : match->getKeywords())
      {
        if (regex.match(keyword).hasMatch())
        {
          matches.emplace_back(match);
        }
      }
    }
  }
  results.sendMatches(matches);
}

void GenericSourceBase::clearMatches()
{
  matches_.clear();
}

void GenericSourceBase::addMatch(const std::shared_ptr<Match>& match)
{
  matches_.push_back(match);
}

} // namespace Deskhare
