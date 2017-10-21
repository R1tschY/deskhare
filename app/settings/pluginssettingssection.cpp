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

#include "pluginssettingssection.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QPluginLoader>
#include <QHBoxLayout>
#include <QListView>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <cpp-utils/algorithm/between.h>

#include "../widgets/gridlayoututils.h"

namespace Deskhare {

class PluginsSettingsModel : public QAbstractListModel
{
public:
  PluginsSettingsModel(const PluginManager* plugin_manager, QObject* parent);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
  const PluginManager* plugin_manager_;
};

PluginsSettingsModel::PluginsSettingsModel(
  const PluginManager* plugin_manager,
  QObject* parent)
: QAbstractListModel(parent), plugin_manager_(plugin_manager)
{ }

int PluginsSettingsModel::rowCount(const QModelIndex& parent) const
{
  return plugin_manager_->getTopLevelPlugins().size();
}

QVariant PluginsSettingsModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return { };

  int row = index.row();
  auto& plugins = plugin_manager_->getTopLevelPlugins();
  if (!cpp::in_range(row, 0, int(plugins.size())))
    return { };

  switch (role)
  {
  case Qt::DisplayRole: {
    QJsonObject metaData = plugins[row].loader->metaData();
    QJsonValue value = metaData.value(QLatin1String("MetaData"))
      .toObject().value("name");
    return value.toString(tr("<Unknown>"));
  }
  case Qt::DecorationRole:
    return { };
  }

  return { };
}

Qt::ItemFlags PluginsSettingsModel::flags(const QModelIndex& index) const
{
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
}


PluginsSettingsSection::PluginsSettingsSection(
  const PluginManager* plugin_manager
)
: SettingsSection(tr("Plugins"), QIcon(":/deskhare/app/puzzle-piece")),
  plugin_manager_(plugin_manager)
{
  auto* layout = new QHBoxLayout();

  view_ = new QListView();
  model_ = new PluginsSettingsModel(plugin_manager, this);
  view_->setModel(model_);

  auto* dummy = new QWidget(this);
  auto* gridLayout = new QGridLayout(this);
  dummy->setLayout(gridLayout);
  gridLayout->setColumnMinimumWidth(0, 50);
  gridLayout->setColumnMinimumWidth(1, 100);

  gridLayout->addWidget(new QLabel(tr("Name:")), 0, 0);
  name_label_ = new QLabel("-");
  gridLayout->addWidget(name_label_, 0, 1);

  gridLayout->addWidget(new QLabel(tr("Id:")), 1, 0);
  id_label_ = new QLabel("-");
  gridLayout->addWidget(id_label_, 1, 1);

  // last spacer row
  GridLayoutUtils::addFillerRow(gridLayout);

  connect(
    view_, &QAbstractItemView::clicked,
    this, &PluginsSettingsSection::activated);

  layout->addWidget(view_, 1);
  layout->addWidget(dummy, 2);
  setLayout(layout);
}

void PluginsSettingsSection::activated(const QModelIndex& index)
{
  int row = index.row();
  auto& plugins = plugin_manager_->getTopLevelPlugins();
  QJsonObject metaData = plugins[row].loader->metaData();
  QJsonObject pluginMeta = metaData.value(QLatin1String("MetaData")).toObject();

  name_label_->setText(pluginMeta.value("name").toString(tr("<Unknown>")));
  id_label_->setText(pluginMeta.value("id").toString(tr("-")));
}

} // namespace Deskhare
