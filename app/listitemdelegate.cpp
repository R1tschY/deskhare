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

#include <listitemdelegate.h>

#include <QStyleOptionViewItem>

namespace Deskhare {

void ListItemDelegate::paint(QPainter* painter,
  const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  return QStyledItemDelegate::paint(painter, option, index);
}

QSize ListItemDelegate::sizeHint(const QStyleOptionViewItem& option,
  const QModelIndex& index) const
{
  auto result = QStyledItemDelegate::sizeHint(option, index);
  result.setHeight(50); // TODO: DPI?
  return result;
}

} // namespace Deskhare

