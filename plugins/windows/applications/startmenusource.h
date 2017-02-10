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

#ifndef APP_PLUGINS_QUICKSTARTER_STARTMENUSOURCE_H_
#define APP_PLUGINS_QUICKSTARTER_STARTMENUSOURCE_H_

#include <QSqlDatabase>
#include "../../../libdeskhare/source.h"

namespace Deskhare {

/// \brief
class StartMenuSource : public Source
{
public:
  StartMenuSource();

  bool canHandleQuery(const Query& query) override;
  std::vector<MatchResult> search(const Query& query) override;

private:
  QSqlDatabase db_;

  void update();
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_STARTMENUSOURCE_H_ */
