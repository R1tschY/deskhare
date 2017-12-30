/// \file utils.cpp

#include <windows.h>
#include <QString>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>

#include "comobject.h"
#include "env.h"
#include "qthelper.h"

namespace WinQt {

QString expandEnvironmentStrings(const QString& path)
{
  const QRegularExpression regex(QStringLiteral("%[^%]+%"));
  QString result;
  int pos = 0;

  auto iter = regex.globalMatch(path);
  while (iter.hasNext()) {
    QRegularExpressionMatch match = iter.next();

    const int start = match.capturedStart();
    const int end = match.capturedEnd();
    const auto envName = path.mid(
      match.capturedStart() + 1, match.capturedLength() - 2).toUpper();
    const auto envValue = qgetenv(envName.toLocal8Bit().data());
    if (!envValue.isNull())
    {
      const auto envStr = QString::fromLocal8Bit(envValue);
      result.reserve(result.size() + envStr.size() + path.size() - end);
      result += QStringRef(&path, pos, start - pos);
      result += envStr;
      pos = end;
    }
  }

  result += QStringRef(&path, pos, path.size() - pos);
  return result;
}

} // namespace WinQt

