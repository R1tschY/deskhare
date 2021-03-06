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

#include "settingswindow.h"

#include <QAbstractListModel>

#include <libdeskhare/pluginmanager.h>


class QListView;
class QLabel;

namespace Deskhare {

class PluginsSettingsModel;

/// \brief
class PluginsSettingsSection: public SettingsSection
{
public:
  PluginsSettingsSection(const PluginManager* plugin_manager);

private:
  QListView* view_;
  PluginsSettingsModel* model_;
  const PluginManager* plugin_manager_;

  QLabel* name_label_;
  QLabel* id_label_;

  void activated(const QModelIndex& index);
};

} // namespace Deskhare
