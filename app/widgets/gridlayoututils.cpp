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

#include "gridlayoututils.h"

#include <QGridLayout>
#include <QSpacerItem>

namespace Deskhare {
namespace GridLayoutUtils {

void addFillerRow(QGridLayout* grid_layout)
{
  int lastRow = grid_layout->rowCount();
  grid_layout->addItem(
    new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::Ignored),
    lastRow, 0, 2, 1);
  grid_layout->setRowStretch(lastRow, 1);
}

}} // namespace Deskhare::GridLayoutUtils
