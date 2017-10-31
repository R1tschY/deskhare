/// \file iconlocation.cpp

#include "iconlocation.h"

#include <QtWin>
#include <QPixmap>
#include <QDebug>
#include <shellapi.h>
#include <windows.h>
#include <winerror.h>

#include "../common/qthelper.h"

namespace WinQt {

QPixmap IconLocation::resolve(IconSizeCategory cat) const
{
  if (path_.isEmpty())
    return QPixmap();

  HICON icon;
  HRESULT res;

  if (cat == IconSizeCategory::Large)
  {
    res = ExtractIconExW(winStringView(path_), index_, &icon, nullptr, 1);
  }
  else
  {
    res = ExtractIconExW(winStringView(path_), index_, nullptr, &icon, 1);
  }

  if (res == 1)
  {
    QPixmap result(QtWin::fromHICON(icon));
    DestroyIcon(icon);
    return result;
  }
  else
  {
    qWarning()
      << "ExtractIconEx failed:"
      << QtWin::stringFromHresult(HRESULT_FROM_WIN32(GetLastError()));
    return QPixmap();
  }
}

QString IconLocation::toString() const
{
  return path_ + QChar(L':') + QString::number(index_);
}

IconLocation IconLocation::fromString(const QString& iconLoc)
{
  int colon = iconLoc.lastIndexOf(QChar(L':'));
  if (colon == -1)
    return IconLocation(); // TODO

  if (colon + 1 >= iconLoc.size())
    return IconLocation(iconLoc.mid(0, colon), -1); // TODO

  bool ok = true;
  int index = QStringRef(&iconLoc, colon + 1, iconLoc.size() - colon - 1).toLong(&ok);
  if (!ok)
    return IconLocation();

  return IconLocation(iconLoc.mid(0, colon), index);
}

} // namespace WinQt

