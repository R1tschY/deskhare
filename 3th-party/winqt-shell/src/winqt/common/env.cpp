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
  QRegularExpression regex(QLatin1String("%[^%]+%"));
  QString result = path;

  auto iter = regex.globalMatch(path);
  int diff = 0;
  while (iter.hasNext()) {
    QRegularExpressionMatch match = iter.next();

    int start = match.capturedStart() + diff + 1;
    int end = match.capturedEnd() + diff - 1;
    auto envName = QStringRef(&result, start, end - start).toString().toUpper();
    auto envValue = qgetenv(envName.toLocal8Bit().data());
    if (!envValue.isNull())
    {
      auto envStr = QString::fromLocal8Bit(envValue);
      result.replace(start - 1, end - start + 2, envStr);
      diff += envStr.size() - (end - start) - 2;
    }
  }

  return result;
}

} // namespace WinQt

