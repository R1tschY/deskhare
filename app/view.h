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

#include <QWidget>

#include <libdeskhare/match.h>

class QAbstractItemModel;
class QLineEdit;

namespace Deskhare {

/// \brief
class View : public QWidget
{
  Q_OBJECT
public:
  enum class Mode
  {
    Nothing,
    Query,
    SearchAction
  };


  using QWidget::QWidget;

  virtual void setResultListModel(QAbstractItemModel* model) = 0;
  virtual QLineEdit* getEdit() = 0;
  virtual QWidget* getResultList() = 0;
  virtual QWidget* getActionList() = 0;
  virtual void setMode(Mode mode) = 0;

signals:
  void openSettings();
};

} // namespace Deskhare
