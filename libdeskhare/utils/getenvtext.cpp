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

#include "getenvtext.h"

#include <stdlib.h>
#include <QString>
#include <QFile>
#include <cpp-utils/algorithm/length.h>

namespace Deskhare {

QString getEnvText(const char* name, const QString& default_)
{
#ifdef Q_OS_WIN
    const QString utf18name = QString::fromLatin1(name);

    std::size_t resultSize = 0;
    _wgetenv_s(&resultSize, 0, 0,
      reinterpret_cast<const wchar_t*>(utf18name.utf16()));
    if (resultSize == 0)
        return default_;

    QString result;
    result.resize(int(resultSize));
    _wgetenv_s(
      &resultSize, reinterpret_cast<wchar_t *>(result.data()), resultSize,
      reinterpret_cast<const wchar_t*>(utf18name.utf16()));

    // requiredSize includes the terminating null, which we don't want.
    Q_ASSERT(result.endsWith(QLatin1Char('\0')));
    result.chop(1);
    return result;

#else
    QByteArray value = qgetenv(name);
    if (value.isNull())
        return default_;
    return QFile::decodeName(value);
#endif
}

} // namespace Deskhare

