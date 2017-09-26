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

#include "settingswindow.h"

#include <QListView>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <cpp-utils/algorithm/between.h>
#include <cpp-utils/to.h>
#include <libdeskhare/controller.h>

#include "pluginssettingssection.h"

namespace Deskhare {

SettingsSection::SettingsSection(
  QString name,
  QIcon icon,
  QWidget* parent
)
: QWidget(parent), name_(name), icon_(icon)
{ }


int SettingsSectionsModel::rowCount(const QModelIndex& parent) const
{
  return sections_.size();
}

QVariant SettingsSectionsModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return { };

  int row = index.row();
  if (!cpp::in_range(row, 0, int(sections_.size())))
    return { };

  switch (role)
  {
  case Qt::DisplayRole:
    return sections_[row]->getName();
  case Qt::DecorationRole:
    return sections_[row]->getIcon();
  }

  return { };
}

Qt::ItemFlags SettingsSectionsModel::flags(const QModelIndex& index) const
{
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
}

void SettingsSectionsModel::append(SettingsSection* section)
{
  beginInsertRows(QModelIndex(), sections_.size(), sections_.size());
  sections_.emplace_back(section);
  endInsertRows();
}

SettingsWindow::SettingsWindow(Controller* controller)
{
  auto* layout = new QHBoxLayout();

  sections_view_ = new QListView();
  layout->addWidget(sections_view_);
  connect(
    sections_view_, &QListView::activated,
    this, &SettingsWindow::selectSection
  );

  settings_stack_ = new QStackedWidget();
  layout->addWidget(settings_stack_);

  sections_model_ = new SettingsSectionsModel(this);
  addSettingsSection(new PluginsSettingsSection(
    controller->getPluginManager()
  ));
  sections_view_->setModel(sections_model_);

  setLayout(layout);
}

void SettingsWindow::addSettingsSection(SettingsSection* section)
{
  sections_model_->append(section);
  settings_stack_->addWidget(section);
}

void SettingsWindow::selectSection(const QModelIndex& index)
{
  if (index.isValid())
    settings_stack_->setCurrentIndex(index.row());
}

} // namespace Deskhare
