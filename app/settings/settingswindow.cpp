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

#include <QSplitter>
#include <QListView>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QProxyStyle>
#include <cpp-utils/algorithm/between.h>
#include <cpp-utils/to.h>
#include <libdeskhare/controller.h>

#include "pluginssettingssection.h"
#include "appsettings.h"

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
  auto* splitter = new QSplitter();

  sections_view_ = new QListView();
  sections_view_->setFrameStyle(QFrame::NoFrame);
  sections_view_->setIconSize(QSize(16, 16));
  sections_view_->setSelectionMode(QAbstractItemView::SingleSelection);
  sections_view_->setSelectionBehavior(QAbstractItemView::SelectRows);
  sections_view_->setStyleSheet(QLatin1String(
    "QListView::item {"
    "  padding: 10px;"
    "  font-size: 12pt;"
    "}"
  ));
//  auto sizePolicy = sections_view_->sizePolicy();
//  sizePolicy.setHorizontalStretch(0);
//  sections_view_->setSizePolicy(sizePolicy);
  splitter->addWidget(sections_view_);

  settings_stack_ = new QStackedWidget();
  auto sizePolicy = settings_stack_->sizePolicy();
  sizePolicy.setHorizontalStretch(1);
  settings_stack_->setSizePolicy(sizePolicy);
  splitter->addWidget(settings_stack_);

  sections_model_ = new SettingsSectionsModel(this);
  addSettingsSection(new AppSettings());
  addSettingsSection(new PluginsSettingsSection(
    controller->getPluginManager()
  ));
  sections_view_->setModel(sections_model_);

  auto* selectionModel = new QItemSelectionModel(sections_model_);
  sections_view_->setSelectionModel(selectionModel);
  connect(
    selectionModel, &QItemSelectionModel::currentChanged,
    this, &SettingsWindow::selectSection
  );
  sections_view_->setCurrentIndex(sections_model_->index(0));

  auto* layout = new QHBoxLayout();
  layout->addWidget(splitter);
  setLayout(layout);

  resize(600, 400);
  splitter->setSizes({150, 450});
}

void SettingsWindow::addSettingsSection(SettingsSection* section)
{
  sections_model_->append(section);
  settings_stack_->addWidget(section);
}

void SettingsWindow::selectSection(
  const QModelIndex& index, const QModelIndex& old)
{
  if (index.isValid())
    settings_stack_->setCurrentIndex(index.row());
}

} // namespace Deskhare
