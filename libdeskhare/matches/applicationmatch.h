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

#ifndef APP_PLUGINS_QUICKSTARTER_MATCHES_APPLICATIONMATCH_H_
#define APP_PLUGINS_QUICKSTARTER_MATCHES_APPLICATIONMATCH_H_

#include <QString>
#include "../match.h"

namespace Deskhare {

class ApplicationMatch : public Match
{
public:
  ApplicationMatch(
    const QString& uri,
    const QString& title,
    const QString& description,
    const QString& file,
    const QString& command_line,
    const QString& working_directory,
    float score);

private:
  const QString file_;
  const QString command_line_;
  const QString working_directory_;
  const QString uri_;
  const QString title_;
  const QString description_;
  const QIcon icon_;
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_MATCHES_APPLICATIONMATCH_H_ */
