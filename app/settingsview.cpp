///
/// Copyright (c) 2017 R1tschY
/// 
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to 
/// deal in the Software without restriction, including without limitation the 
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.
///

#include "app/settingsview.h"

#include <QListWidget>
#include <QStackedLayout>
#include <QLabel>
#include <QFrame>

namespace Deskhare {

SettingsView::SettingsView(QWidget* parent)
: QWidget(parent)
{
  widget_stack_ = new QStackedLayout(this);

  createOverview();
}

void SettingsView::createOverview()
{
  auto* overview = new QWidget(this);
  overview->setBackgroundRole(QPalette::Base);
  widget_stack_->addWidget(overview);
  auto* overview_layout = new QVBoxLayout(overview);


  overview->setStyleSheet(
  "QWidget {"
  "    background: #FFF;"
  "}");



  for (const QString& group : {"Group 1", "Group 2"})
  {
    auto* frame = new QFrame();
    frame->setFrameStyle(QFrame::HLine);
    overview_layout->addWidget(frame);
    overview_layout->addWidget(new QLabel(group));

    auto* listview = new QListWidget(overview);
    listview->setFrameStyle(QFrame::NoFrame);
    listview->setViewMode(QListView::IconMode);
    listview->setIconSize(QSize(64, 64));
    listview->setWrapping(true);
    listview->setResizeMode(QListView::Adjust);
    listview->setSpacing(10);
    listview->setWordWrap(true);
    overview_layout->addWidget(listview);
    new QListWidgetItem(
      QIcon::fromTheme("preferences-desktop-display"),
      "Anzeige", listview);
    new QListWidgetItem(
      QIcon::fromTheme("preferences-desktop-display"),
      "Anzeige 2", listview);
  }
}

} // namespace Deskhare
