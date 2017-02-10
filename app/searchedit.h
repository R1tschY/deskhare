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

#pragma once

#include <QLineEdit>

namespace Deskhare {

class SearchEdit : public QLineEdit
{
  Q_OBJECT
public:
  SearchEdit(QWidget* parent = 0);

  QString getQuery() const { return offscreen_edit_.text(); }
  void clearQuery();

signals:
  void queryChanged();

private slots:
  void onQueryChanged();

protected:
  void keyPressEvent(QKeyEvent* evt) override;

private:
  QLineEdit offscreen_edit_;
};

} // namespace Deskhare
