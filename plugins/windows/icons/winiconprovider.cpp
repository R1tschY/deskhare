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

#include "winiconprovider.h"

#include <QIcon>
#include <QDebug>

#include <winqt/shell/iconlocation.h>

namespace Deskhare {

WinIconProvider::WinIconProvider()
{
  fillLocations();
}

float WinIconProvider::getPriorityIndex()
{
  return 10;
}

QIcon WinIconProvider::iconFromTheme(const QString& name)
{
  qDebug() << "request" << name;
  if (cache_.contains(name))
  {
    qDebug() << "in cache" << name;
    return *cache_.object(name);
  }

  QIcon* icon = resolve(name);
  QIcon result = *icon;
  cache_.insert(name, icon);
  return result;
}

QString WinIconProvider::getDescription() const
{
  return "Windows icon provider.";
}

QIcon* WinIconProvider::resolve(const QString& name) const
{
  qDebug() << "resolve" << name;
  auto locationsIter = icon_locations_.find(name);
  if (locationsIter == icon_locations_.end()) {
    qDebug() << "not found" << name;
    return new QIcon();
  }

  QIcon* result = nullptr;
  const auto locations = *locationsIter;
  for (auto& location : locations)
  {
    qDebug() << "search for" << name << "@" << location;
    auto iconLoc = WinQt::IconLocation::fromString(location);
    if (!iconLoc.isNull())
    {
      auto pixmap = iconLoc.resolve();
      if (!pixmap.isNull())
      {
        qDebug() << "got" << name << "@" << location;
        result = new QIcon(iconLoc.resolve());
        break;
      }
    }
  }

  return result ? result : new QIcon();
}

void WinIconProvider::fillLocations()
{
  // rename: imageres.dll:-90
  // delete: imageres.dll:-162/-85/-260
  // no: imageres.dll:-208
  // copy-files: imageres.dll:-245/shell32.dll:-135
  // move-files: imageres.dll:-244/shell32.dll:-133/-69
  // document-properties: shell32.dll:-70
  // select-all: imageres.dll:-249
  // select-nothing: imageres.dll:-250
  // select-inv: imageres.dll:-251
  // redo: imageres.dll:-252
  // sort: imageres.dll:-253
  // insert-column: imageres.dll:-254
  // undo: imageres.dll:-256
  // convert: imageres.dll:-297
  // column-anpassen: imageres.dll:-306
  // new-folder: imageres.dll:-4/-5
  // search-in-folder: imageres.dll:-14
  // search-in-file: shell32.dll:-56
  // settings: shell32.dll:-58/-208/-166
  // play: shell32.dll:-138
  // red-ok: shell32.dll:-145
  // upload/share/send-to: shell32.dll:-147
  // fullscreen: shell32.dll:-160
  // search: shell32.dll:-210
  // favs: shell32.dll:-209
  // groupby: shell32.dll:-246
  // ok: shell32.dll:-297/-303
  // go-right: shell32.dll:-300
  // go-up: shell32.dll:-308
  // refresh: shell32.dll:-239

  icon_locations_.insert("accessories-calculator", {"calc.exe:0"});

  icon_locations_.insert("dialog-apply", {});

  icon_locations_.insert("document-print", {});
  icon_locations_.insert("document-print-preview", {});
  icon_locations_.insert("document-properties", {});
  icon_locations_.insert("document-open", {});
  icon_locations_.insert("document-new", {});
  icon_locations_.insert("document-save", {});
  icon_locations_.insert("document-save-as", {});
  icon_locations_.insert("document-save", {});

  icon_locations_.insert("edit-undo", {"shell32.dll:-298"});
  icon_locations_.insert("edit-redo", {});
  icon_locations_.insert("edit-copy", {"shell32.dll:-135"});
  icon_locations_.insert("edit-paste", {"shell32.dll:-261"});
  icon_locations_.insert("edit-cut", {"shell32.dll:-260"});
  icon_locations_.insert("edit-delete", {});

  icon_locations_.insert("folder-new", {});
  icon_locations_.insert("folder-copy", {});
  icon_locations_.insert("folder-move", {});

  icon_locations_.insert("go-bottom", {});
  icon_locations_.insert("go-down", {});
  icon_locations_.insert("go-first", {});
  icon_locations_.insert("go-home", {});
  icon_locations_.insert("go-jump", {});
  icon_locations_.insert("go-last", {});
  icon_locations_.insert("go-next", {});
  icon_locations_.insert("go-previous", {});
  icon_locations_.insert("go-top", {});
  icon_locations_.insert("go-up", {});

  icon_locations_.insert("help-about", {});
  icon_locations_.insert("help-contents", {});
  icon_locations_.insert("help-hint", {});

  icon_locations_.insert("list-add", {});
  icon_locations_.insert("list-remove", {});
  icon_locations_.insert("list-add", {});

}

} // namespace Deskhare

