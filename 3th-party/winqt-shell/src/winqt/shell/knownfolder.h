#pragma once

#include <QString>
#include <shlobj.h>

namespace WinQt {

QString getKnownFolderPath(const KNOWNFOLDERID& id);

} // namespace WinQt
