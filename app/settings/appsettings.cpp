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

#include "appsettings.h"

#include <QGridLayout>
#include <QWidgetItemV2>
#include <QLabel>
#include <QFont>

#include "../widgets/switch.h"
#include "../widgets/gridlayoututils.h"

namespace Deskhare {

AppSettings::AppSettings()
: SettingsSection(tr("General"), QIcon(QLatin1String(":/deskhare/app/browser")))
{
  auto* layout = new QGridLayout();

  QFont boldFont = font();
  boldFont.setBold(true);

  layout->setColumnMinimumWidth(0, 5);
  layout->setColumnStretch(0, 0);
  layout->setColumnStretch(1, 1);
  layout->setColumnStretch(2, 1);

  addGroupHeader(layout, tr("Application"), boldFont);

  auto* autostartSwitch = new Widgets::Switch();
  addRow(layout, tr("Startup on login"), autostartSwitch);

  GridLayoutUtils::addFillerRow(layout);

  setLayout(layout);
}

void AppSettings::addGroupHeader(
  QGridLayout* layout, const QString& name, const QFont& font)
{
  int rowCount = layout->rowCount();

  auto* group_label = new QLabel(name);
  group_label->setFont(font);
  layout->addItem(new QWidgetItemV2(group_label), rowCount, 0, 1, 3);
}

void AppSettings::addRow(
  QGridLayout* layout, const QString& name, QWidget* widget)
{
  int rowCount = layout->rowCount();

  layout->addWidget(new QLabel(name), rowCount, 1);
  layout->addWidget(widget, rowCount, 2);
}

} // namespace Deskhare

