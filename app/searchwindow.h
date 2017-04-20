//
// deskhare - cross-platform quick launcher
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

#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QWidget>
#include <memory>

#include <libdeskhare/controller.h>

class QLineEdit;
class QListView;
class QLabel;
class QxtGlobalShortcut;
class QModelIndex;
class QStackedWidget;
class QTimer;

namespace Deskhare {

class SearchEdit;
class MatchesModel;
class SettingsView;

class SearchWindow : public QWidget
{
  Q_OBJECT
public:
  explicit SearchWindow(QWidget *parent = 0);
  ~SearchWindow();

public slots:
  void onEdit();

  std::shared_ptr<Action> getAction() const;
  std::shared_ptr<Match> getMatch() const;

protected:
  void keyPressEvent(QKeyEvent *) override;
  void hideEvent(QHideEvent *) override;
  void setVisible(bool visible) override;

  bool eventFilter(QObject* watched, QEvent* event) override;
  bool event(QEvent *event) override;

private:
  QLineEdit* edit_;
  QListView* list_;
  QListView* actions_list_;
  QStackedWidget* result_lists_;
  QueryResultModel* model_;
  QueryResultModel* actions_model_;
  QxtGlobalShortcut* shortcut_;
  QTimer* debounce_timer_;
  std::unique_ptr<SettingsView> settings_view_;

  Controller controller_;

  void configureResultList(QListView* list);
  void clearResult();

private slots:
  void toggleVisibility();
  void toggleResultList();
  void escapeState();
  void onResultActivated(const QModelIndex &index);
  void onStartSearch();
};

} // namespace Deskhare

#endif // SEARCHWINDOW_H
