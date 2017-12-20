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

#pragma once

#include <action.h>

#include <QCoreApplication>

namespace Deskhare {

/// \brief
class CopyTextAction: public Action
{
  Q_DECLARE_TR_FUNCTIONS(CopyTextAction)
public:
  CopyTextAction();
  CopyTextAction(const QString& text);

  static std::shared_ptr<CopyTextAction> getInstance();

  bool canHandleMatch(const Match& match) const override;
  void execute(const Match& target) const override;

private:
  QString text_;
};

} // namespace Deskhare
