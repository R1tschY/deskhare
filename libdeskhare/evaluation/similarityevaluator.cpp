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

#include "similarityevaluator.h"

#include <cmath>
#include <algorithm>
#include <QDebug>

#include "../query.h"
#include "../match.h"

namespace Deskhare {

float Deskhare::SimilarityEvaluator::eval(
  const Query& query,
  const Match& match) const
{
  // everything must be thread-safe

  QString querystr = query.getSearchString();
  QString matched = match.getTitle();

  if (querystr.length() == 0 || matched.length() == 0)
    // no evaluation possible
    return 1.0;

  int index = matched.indexOf(querystr, 0, Qt::CaseInsensitive);
  if (index != -1)
  {
    return std::max(std::exp(-index / 100.f), 0.8f);
  }
  else if (matched[0].toLower() == querystr[0].toLower())
  {
    return 0.75;
  }

  return 0.5;
}

} // namespace Deskhare
