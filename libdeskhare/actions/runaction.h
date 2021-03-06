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

#ifndef APP_PLUGINS_QUICKSTARTER_ACTIONS_RUNACTION_H_
#define APP_PLUGINS_QUICKSTARTER_ACTIONS_RUNACTION_H_

#include "../action.h"
#include <QCoreApplication>

namespace Deskhare {

class RunAction : public Action
{
  Q_DECLARE_TR_FUNCTIONS(RunAction)
public:
  RunAction(const Action& action);

  bool canHandleMatch(const Match& match) const override;
  void execute(const Match& match) const override;

private:
  std::shared_ptr<const Action> action_;
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_ACTIONS_RUNACTION_H_ */
