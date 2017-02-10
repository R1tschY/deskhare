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

#include "searchedit.h"

#include <QKeyEvent>

namespace Deskhare {

SearchEdit::SearchEdit(QWidget* parent)
: QLineEdit(parent)
{
  setReadOnly(true);

  connect(
    &offscreen_edit_, SIGNAL(textChanged(QString)),
    this, SLOT(onQueryChanged()));
}

void SearchEdit::clearQuery()
{
  offscreen_edit_.clear();
}

void SearchEdit::keyPressEvent(QKeyEvent* evt)
{
  offscreen_edit_.event(evt);
}

void SearchEdit::onQueryChanged()
{
  emit queryChanged();
}

} // namespace Deskhare
