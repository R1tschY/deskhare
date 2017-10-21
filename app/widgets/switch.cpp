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

#include "widgets/switch.h"

#include <QPainter>
#include <QMouseEvent>

namespace Deskhare {
namespace Widgets {

Switch::Switch(QWidget *parent)
: QAbstractButton(parent),
  height_(16),
  opacity_(0.000),
  state_(false),
  margin_(3),
  thumb_("#d5d5d5"),
  animation_(new QPropertyAnimation(this, "offset", this))
{
  setOffset(height_ / 2);
  y_ = height_ / 2;
  setBrush(QColor("#009688"));
  setCursor(Qt::PointingHandCursor);
  setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
}

Switch::Switch(const QBrush &brush, QWidget *parent)
  :
    QAbstractButton(parent),
    height_(16),
    state_(false),
    opacity_(0.000),
    margin_(3),
    thumb_(Qt::green), // "#d5d5d5"),
    animation_(new QPropertyAnimation(this, "offset", this))
{
  setOffset(height_ / 2);
  y_ = height_ / 2;
  setBrush(brush);
}

void Switch::paintEvent(QPaintEvent *e)
{
  QPainter p(this);
  p.setPen(Qt::NoPen);
  if (isEnabled())
  {
    p.setBrush(state_ ? brush() : Qt::black);
    p.setOpacity(state_ ? 0.5 : 0.38);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.drawRoundedRect(
      QRect(margin_, margin_, width() - 2 * margin_, height() - 2 * margin_),
      8.0,
      8.0);
    p.setBrush(thumb_);
    p.setOpacity(1.0);
    p.drawEllipse(
      QRectF(offset() - (height_ / 2), y_ - (height_ / 2), height(), height()));
  }
  else
  {
    p.setBrush(Qt::black);
    p.setOpacity(0.12);
    p.drawRoundedRect(
      QRect(margin_, margin_, width() - 2 * margin_, height() - 2 * margin_),
      8.0,
      8.0);
    p.setOpacity(1.0);
    p.setBrush(Qt::red);//QColor("#BDBDBD"));
    p.drawEllipse(
      QRectF(offset() - (height_ / 2), y_ - (height_ / 2), height(), height()));
  }
}

void Switch::mouseReleaseEvent(QMouseEvent *e)
{
  if (e->button() & Qt::LeftButton)
  {
    state_ = !state_;
    thumb_ = state_ ? _brush : QBrush("#d5d5d5");
    if (state_)
    {
      animation_->setStartValue(height_ / 2);
      animation_->setEndValue(width() - height_);
      animation_->setDuration(120);
      animation_->start();
    }
    else
    {
      animation_->setStartValue(offset());
      animation_->setEndValue(height_ / 2);
      animation_->setDuration(120);
      animation_->start();
    }
  }
  QAbstractButton::mouseReleaseEvent(e);
}

QSize Switch::sizeHint() const
{
  return QSize(2 * (height_ + margin_), height_ + 2 * margin_);
}

} // namespace Widgets
} // namespace Deskhare
