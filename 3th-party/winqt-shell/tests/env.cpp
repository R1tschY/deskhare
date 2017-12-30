/// \file env.cpp

#include "env.h"

#include <windows.h>
#include <QtTest>
#include <QtGlobal>
#include <winqt/common/env.h>
#include <winqt/common/qthelper.h>

namespace WinQt {

namespace{

QString expandEnvironmentStringsRef(const QString& str)
{
  wchar_t buffer[32 * 1024];
  auto size = ExpandEnvironmentStringsW(
    WinQt::winStringView(str), buffer, ARRAYSIZE(buffer));

  if (size)
    return QString::fromWCharArray(buffer, size - 1);
  else
    return QString();
}

}  // namespace

void EnvTest::testExpandEnvironmentStrings()
{
  qputenv("ABC", "testVALUE");
  qputenv("DEF", "TeSt");

  QCOMPARE(
    WinQt::expandEnvironmentStrings("%ABC%"),
    QString("testVALUE")
  );

  QCOMPARE(
    WinQt::expandEnvironmentStrings("abc%ABC%ABC"),
    QString("abctestVALUEABC")
  );

  QCOMPARE(
    WinQt::expandEnvironmentStrings("%ABC%ABC"),
    QString("testVALUEABC")
  );

  QCOMPARE(
    WinQt::expandEnvironmentStrings("abc%ABC%"),
    QString("abctestVALUE")
  );

  QCOMPARE(
    WinQt::expandEnvironmentStrings("abc"),
    QString("abc")
  );

  QCOMPARE(
    WinQt::expandEnvironmentStrings(""),
    QString("")
  );

  QCOMPARE(
    WinQt::expandEnvironmentStrings(QString()),
    QString()
  );

  QCOMPARE(
    WinQt::expandEnvironmentStrings("%ABC%%ABC%%ABC%"),
    QString("testVALUEtestVALUEtestVALUE")
  );

  QCOMPARE(
    WinQt::expandEnvironmentStrings("-%ABC%__%DEF%/"),
    QString("-testVALUE__TeSt/")
  );

  QCOMPARE(
    WinQt::expandEnvironmentStrings("-%AbC%__%dEf%/"),
    QString("-testVALUE__TeSt/")
  );


  for (QString str : {
    "-%AbC%__%dEf%#%%/",
    "%gfhjbvghkfsdghkf%%abc%%%",
    "%",
    "%%",
    "%%%",
    "**%%%~~",
    "%abc"
    "%abc%%",
    "%Path%%"
  })
  {
    QCOMPARE(
      WinQt::expandEnvironmentStrings(str),
      expandEnvironmentStringsRef(str)
    );
  }
}

} // namespace WinQt

QTEST_MAIN(WinQt::EnvTest)
