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

#include "calculatorsource.h"

#include <libdeskhare/match.h>
#include <libdeskhare/query.h>
#include <libdeskhare/resultset.h>
#include <libdeskhare/matches/textmatch.h>
#include <libdeskhare/shell/iconservice.h>

#include "calcparser.h"

namespace Deskhare {

CalculatorSource::CalculatorSource(const PluginContext& ctx)
{
}

bool CalculatorSource::canHandleQuery(const Query& query)
{
  return query.hasCategory(Query::Category::Meta);
}

void CalculatorSource::search(const Query& query, ResultSet& results)
{
  QString query_string = query.getSearchString();

  double result;
  if (Calc::evalExpression(query_string, result))
  {
    auto result_str = QString::number(result);
    results.sendMatch(std::make_shared<TextMatch>(
      MatchScore::Excellent,
      result_str,
      QString("%1 = %2").arg(query_string, result_str),
      IconService::iconFromTheme("accessories-calculator")
    ));
  }
}

QString CalculatorSource::getDescription() const
{
  return QLatin1String("Calculator."
    ""
    "Evaluates simple formulars.");
}

} // namespace Deskhare
