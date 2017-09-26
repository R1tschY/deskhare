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

#include <QWidget>
#include <QString>
#include <QIcon>
#include <QAbstractListModel>

class QListView;
class QStackedWidget;

namespace Deskhare {

class Controller;

class SettingsSection : public QWidget
{
public:
  explicit SettingsSection(
    QString name,
    QIcon icon,
    QWidget* parent = nullptr
  );

  const QString& getName() const { return name_; }
  const QIcon& getIcon() const { return icon_; }

private:
  QString name_;
  QIcon icon_;
};


class SettingsSectionsModel : public QAbstractListModel
{
public:
  using QAbstractListModel::QAbstractListModel;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;

  void append(SettingsSection* section);

private:
  std::vector<SettingsSection*> sections_;
};


/// \brief
class SettingsWindow : public QWidget
{
public:
  SettingsWindow(Controller* controller);

  void addSettingsSection(SettingsSection* section);

private:
  QListView* sections_view_;
  SettingsSectionsModel* sections_model_;
  QStackedWidget* settings_stack_;

  void selectSection(const QModelIndex& index);
};

} // namespace Deskhare
