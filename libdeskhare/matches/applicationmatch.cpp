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

#include "applicationmatch.h"

namespace Deskhare {

ApplicationMatch::ApplicationMatch(
  const QString& uri,
  const QString& title,
  const QString& description,
  const QString& file,
  const QString& command_line,
  const QString& working_directory,
  float score
)
: Match(uri, score),
  title_(title),
  description_(description),
  file_(file),
  command_line_(command_line),
  working_directory_(working_directory)
{ }

/*
Match::InfoData ApplicationMatch::getInfo(InfoType type) const
{
  Match::InfoData result;
  if (type & Match::Application)
  {
    if (!working_directory_.isEmpty())
      result.emplace_back(QStringLiteral("working directory"), working_directory_);

    result.emplace_back(
          QStringLiteral("command line"),
      QStringLiteral("\"") + file_ + QStringLiteral("\" ") + command_line_);
  }

  if (type & Match::File)
  {
    result.emplace_back(QStringLiteral("file path"), file_);
  }

  return result;
}*/

} // namespace QuickStarter
