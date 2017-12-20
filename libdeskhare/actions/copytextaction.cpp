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

#include "copytextaction.h"

#include <QApplication>
#include <QClipboard>

namespace Deskhare {

CopyTextAction::CopyTextAction()
: Action(
  tr("Copy to clipboard"),
  tr("Copy text of selected item to clipboard"),
  QIcon::fromTheme("edit-copy"),
  0.0)
{ }

CopyTextAction::CopyTextAction(const QString& text)
: CopyTextAction()
{
  text_ = text;

  // empty string encodes reading from match
  if (text_.isNull())
    text_ = QLatin1String("");
}

std::shared_ptr<CopyTextAction> CopyTextAction::getInstance()
{
  static auto instance = std::make_shared<CopyTextAction>();
  return instance;
}

bool CopyTextAction::canHandleMatch(const Match& match) const
{
  return true;
}

void CopyTextAction::execute(const Match& target) const
{
  QString text = text_;
  if (text.isNull()) {
    // using match text
    text = target.getTitle();
  }

  QApplication::clipboard()->setText(text);
}

} // namespace Deskhare
