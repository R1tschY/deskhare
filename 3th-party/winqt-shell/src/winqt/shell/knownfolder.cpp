#include "knownfolder.h"

#include <QDir>

namespace WinQt {

QString getKnownFolderPath(const KNOWNFOLDERID& id)
{
  wchar_t* path;
  QString result;

  HRESULT res =
    ::SHGetKnownFolderPath(id, KF_FLAG_DONT_VERIFY, nullptr, &path);
  if (SUCCEEDED(res))
  {
    result = QDir::fromNativeSeparators(QString::fromWCharArray(path));
    ::CoTaskMemFree(path);
  }
  return result;
}

} // namespace WinQt
