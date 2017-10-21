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
#include <QBrush>
#include <QPropertyAnimation>

namespace Deskhare {
namespace Widgets {

/// \brief
class Switch: public QAbstractButton
{
  Q_OBJECT
  Q_PROPERTY(int offset READ offset WRITE setOffset)
  Q_PROPERTY(QBrush brush READ brush WRITE setBrush)

public:
  Switch(QWidget* parent = nullptr);
  Switch(const QBrush& brush, QWidget* parent = nullptr);

  QSize sizeHint() const override;

  QBrush brush() const
  {
    return _brush;
  }
  void setBrush(const QBrush &brsh)
  {
    _brush = brsh;
  }

  int offset() const
  {
    return _x;
  }
  void setOffset(int o)
  {
    _x = o;
    update();
  }

protected:
  void paintEvent(QPaintEvent*) override;
  void mouseReleaseEvent(QMouseEvent*) override;

private:
  bool state_;
  qreal opacity_;
  int _x, y_, height_, margin_;
  QBrush thumb_, _track, _brush;
  QPropertyAnimation *animation_ = nullptr;
};

} // namespace Widgets
} // namespace Deskhare
