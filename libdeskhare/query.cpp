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

#include "query.h"

#include <QRegularExpression>

namespace Deskhare {

Query::Query(Categories categories, const QString& search_string,
  const Match* target)
: categories_(categories), search_string_(search_string), target_(target)
{ }

QRegularExpression Query::getSearchRegex() const
{
  auto regex = QRegularExpression(
    "\\b" + QRegularExpression::escape(search_string_),
    QRegularExpression::CaseInsensitiveOption
  );
  regex.optimize();
  return regex;
}

} // namespace QuickStarter
