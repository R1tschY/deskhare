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

#include <QObject>
#include <QAbstractButton>

class QPropertyAnimation;

namespace Deskhare {
namespace Widgets {

/// \brief
class Switch: public QAbstractButton
{
  Q_OBJECT
  Q_PROPERTY(int offset READ offset WRITE setOffset)

public:
  Switch(QWidget* parent = nullptr);

  QSize sizeHint() const override;

  int offset() const
  {
    return x_;
  }
  void setOffset(int o)
  {
    x_ = o;
    update();
  }

protected:
  void paintEvent(QPaintEvent*) override;

  void nextCheckState() override;
  void checkStateSet() override;

private:
  int x_, height_, margin_;
  QPropertyAnimation *animation_ = nullptr;
};

} // namespace Widgets
} // namespace Deskhare
