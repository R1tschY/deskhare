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

#ifndef APP_PLUGINS_QUICKSTARTER_SOURCES_ACTION_H_
#define APP_PLUGINS_QUICKSTARTER_SOURCES_ACTION_H_

#include "../libdeskhare/match.h"

namespace Deskhare {

class Action : public Match
{
public:
  Action(
    const QString& title,
    const QString& description,
    const QIcon& icon,
    float score)
  : Match(title, description, icon, QString(), score)
  { }

  Action(
    const QString& title,
    const QString& description,
    const QIcon& icon,
    const QString& uri,
    float score)
  : Match(title, description, icon, uri, score)
  { }

  virtual bool canHandleMatch(const Match& match) const = 0;
  virtual void execute(const Match& target) const = 0;

  std::shared_ptr<Action> getDefaultAction() const override;
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_SOURCES_ACTION_H_ */
