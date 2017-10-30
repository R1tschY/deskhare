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
#include <QDebug>
#include <QPalette>
#include <QPropertyAnimation>

namespace Deskhare {
namespace Widgets {

Switch::Switch(QWidget *parent)
: QAbstractButton(parent),
  height_(22),
  margin_(-2),
  animation_(new QPropertyAnimation(this, "offset", this))
{
  setOffset(height_ / 2);

  setCursor(Qt::PointingHandCursor);
  setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  setCheckable(true);
}

void Switch::paintEvent(QPaintEvent *e)
{
  bool state = isChecked();

  auto& colors = palette();
  auto back_cr = state ? QPalette::Highlight : QPalette::Dark;
  auto thumb_cr = state ? QPalette::Light : QPalette::Light;
  auto color_group = isEnabled() ? QPalette::Normal : QPalette::Disabled;

  QPainter p(this);
  p.setPen(Qt::NoPen);
  p.setRenderHint(QPainter::Antialiasing, true);

  p.setBrush(colors.color(color_group, back_cr));
  p.drawChord(
    QRectF(0, 0, height_, height_),
    90 * 16, 180 * 16);
  p.drawChord(
    QRectF(height_, 0, height_, height_),
    -90 * 16, 180 * 16);
  p.drawRect(
    QRectF(height_ / 2, 0, height_, height_));

  p.setBrush(colors.color(color_group, thumb_cr));
  p.drawEllipse(
    QRectF(
      offset() - (height_ / 2) - margin_, -margin_,
      height_ + 2 * margin_, height_ + 2 * margin_));
}

void Switch::nextCheckState()
{
  QAbstractButton::nextCheckState();
  Switch::checkStateSet();
}

void Switch::checkStateSet()
{
  QAbstractButton::checkStateSet();

  bool state = isChecked();
  if (state)
  {
    animation_->setStartValue(offset());
    animation_->setEndValue(height_ * 2 - height_ / 2);
  }
  else
  {
    animation_->setStartValue(offset());
    animation_->setEndValue(height_ / 2);
  }

  animation_->setDuration(120);
  animation_->start();
}

QSize Switch::sizeHint() const
{
  return QSize(2 * height_, height_);
}

} // namespace Widgets
} // namespace Deskhare
