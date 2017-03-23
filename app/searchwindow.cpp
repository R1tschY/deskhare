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

#include "searchwindow.h"

#include <QDebug>
#include <QString>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>
#include <QStringListModel>
#include <QKeyEvent>
#include <QLabel>
#include <QSettings>
#include <QLoggingCategory>
#include <QCoreApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>
#include <QxtGlobalShortcut>
#include <QTimer>
#include <QFrame>
#include <QPushButton>
#include <QAction>

#include "searchedit.h"
#include "settingsview.h"
#include <libdeskhare/match.h>
#include <libdeskhare/action.h>
#include <libdeskhare/queryresultmodel.h>

Q_LOGGING_CATEGORY(searchWindow, "deskhare.app")

namespace Deskhare {

SearchWindow::~SearchWindow() = default;

SearchWindow::SearchWindow(QWidget *parent)
: QWidget(parent)
{
  QSettings settings;

  setWindowTitle(qAppName());

  setWindowFlags(Qt::Tool
    | Qt::WindowCloseButtonHint
    | Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TranslucentBackground);

  auto* effect = new QGraphicsDropShadowEffect();
  effect->setBlurRadius(10);
  effect->setColor(QColor(0, 0, 0, 0xcc));
  effect->setXOffset(3.0);
  effect->setYOffset(3.0);
  setGraphicsEffect(effect);

  QString hotkey = settings.value("hotkey", "Ctrl+.").toString();
  QKeySequence keyseq(hotkey);
  if (!keyseq.isEmpty())
  {
    qCInfo(searchWindow) << "Set show hotkey to" << keyseq;
    shortcut_ = new QxtGlobalShortcut(this);
    shortcut_->setShortcut(keyseq);
    connect(shortcut_, &QxtGlobalShortcut::activated,
        this, &SearchWindow::toggleVisibility);
  }
  else
  {
    qCInfo(searchWindow) << "Invalid show hotkey:" << hotkey;
  }

  edit_ = new QLineEdit(this);
  edit_->setPlaceholderText(tr("Type to search ..."));
  edit_->installEventFilter(this);
  auto font = edit_->font();
  font.setPointSize(24);
  edit_->setFont(font);
  edit_->setMinimumWidth(1000);
  edit_->setFrame(false);
  edit_->setObjectName("input");
  connect(edit_, SIGNAL(textChanged(QString)),
    this, SLOT(onEdit()));

  model_ = controller.getQueryResultModel();
  list_ = new QListView();
  list_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  list_->setEditTriggers(QAbstractItemView::NoEditTriggers);
  list_->setFocusPolicy(Qt::NoFocus);
  list_->setModel(model_);
  list_->hide();
  list_->setFrameShape(QFrame::NoFrame);
  list_->setObjectName("matches");
  connect(list_, &QAbstractItemView::activated,
    this, &SearchWindow::activated);
  connect(model_, &QAbstractItemModel::rowsInserted,
    this, &SearchWindow::onNewResultRows);

  // window frame
  auto* frame = new QFrame(this);
  frame->setFrameShape(QFrame::NoFrame);
  frame->setObjectName("frame");

  auto* frameLayout = new QVBoxLayout(this);
  frameLayout->setSizeConstraint(QLayout::SetFixedSize);
  frameLayout->addWidget(frame);
  frameLayout->setMargin(0);
  setLayout(frameLayout);

  settings_view_ = std::make_unique<SettingsView>();

  auto* headerLine = new QHBoxLayout();
  headerLine->addWidget(new QLabel(tr("All")), 1.0, Qt::AlignCenter);

  auto* menuButton = new QPushButton();
  menuButton->setIcon(QIcon::fromTheme("configure"));
  menuButton->setIconSize(QSize(16, 16));
  menuButton->setFlat(true);
  menuButton->setObjectName("menuButton");
  menuButton->setFocusPolicy(Qt::NoFocus);
  menuButton->setContextMenuPolicy(Qt::ActionsContextMenu);
  headerLine->addWidget(menuButton, 0.0, Qt::AlignRight);
  connect(
    menuButton, &QPushButton::clicked,
    settings_view_.get(), &SettingsView::show);

//  auto* settingsAction = new QAction("Settings", menuButton);
//  settingsAction->setShortcuts({QKeySequence("Ctrl+,"), QKeySequence("Alt+,")});
//  menuButton->addAction(settingsAction);

  auto* hideAction = new QAction(tr("Hide"), menuButton);
  hideAction->setShortcuts({
    QKeySequence("Esc"),
    QKeySequence("Ctrl+W"),
    QKeySequence("Ctrl+D")});
  connect(hideAction, &QAction::triggered, this, &SearchWindow::hide);
  menuButton->addAction(hideAction);

  auto* quitAction = new QAction(tr("Quit"), menuButton);
  quitAction->setShortcuts({QKeySequence("Alt+F4"), QKeySequence("Ctrl+Q")});
  connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
  menuButton->addAction(quitAction);

  auto* contentLayout = new QVBoxLayout(frame);
  contentLayout->addLayout(headerLine);
  contentLayout->addWidget(edit_);
  contentLayout->addWidget(list_);
  contentLayout->setSpacing(0);
  contentLayout->setMargin(0);
  frame->setLayout(contentLayout);

  setStyleSheet(""
    "#frame {"
    "  background-color: #ccc; "
    "  border-radius: 10px;"
    "  padding: 10px;"
    "  padding-top: 0px;"
    "  margin: 10px;"
    "}"
    ""
    "#input {"
    "  border-radius: 3px;"
    "}");
}

void SearchWindow::onEdit()
{
  list_->hide();

  if (edit_->text().length())
  {
    controller.search(edit_->text());
    list_->show();
    //list_->setCurrentIndex(model_->index(0));
  }
}

void SearchWindow::keyPressEvent(QKeyEvent* evt)
{
  int key = evt->key();
  switch (key)
  {
  case Qt::Key_Escape:
    evt->accept();
    return hide();
  }
}

void SearchWindow::hideEvent(QHideEvent* evt)
{
  edit_->clear();
}

bool SearchWindow::eventFilter(QObject* watched, QEvent* event)
{
  if (event->type() == QEvent::KeyPress)
  {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    switch (keyEvent->key())
    {
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Enter:
    case Qt::Key_Return:
      static_cast<QObject*>(list_)->event(event);
      return true;

    case Qt::Key_Tab:
      // TODO: toggle action view
      return true;

    case Qt::Key_Escape:
      escapeState();
      return true;
    }
  }

  return false;
}

void SearchWindow::toggleVisibility()
{
  setVisible(!isVisible());
  activateWindow();
}

void SearchWindow::escapeState()
{
  // TODO: quit action view
  if (!edit_->text().isEmpty())
  {
    edit_->clear();
  }
  else
  {
    close();
  }
}

void SearchWindow::setVisible(bool visible)
{
  // Skip if nothing to do
  if (isVisible() == visible)
      return;

  QWidget::setVisible(visible);

  if (visible)
  {
    // move to screen center
    QDesktopWidget *desktop = QApplication::desktop();
    int screen = desktop->screenNumber(QCursor::pos());
    QSize windowSize = size();
    move(desktop->availableGeometry(screen).center()
      - QPoint(windowSize.width()/2, windowSize.height()/2));

    raise();
    activateWindow();
  }
  else
  {
    edit_->clear();
  }
}

bool SearchWindow::event(QEvent* event)
{
  if (event->type() == QEvent::WindowDeactivate)
  {
    QTimer::singleShot(0, Qt::CoarseTimer, this, &SearchWindow::hide);
  }

  return QWidget::event(event);
}

void SearchWindow::onNewResultRows()
{
  if (list_->currentIndex().isValid())
    return;

  // if no selection, select first row
  list_->setCurrentIndex(model_->index(0));
}

void SearchWindow::activated(const QModelIndex& index)
{
  if (!index.isValid())
    return;

  auto* match = model_->getMatch(index.row());
  if (!match)
    return;

  controller.execute(*match);
}

} // namespace Deskhare
