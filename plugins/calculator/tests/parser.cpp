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

#include "parser.h"

#include <QtTest>

#include "../calcparser.h"

namespace Deskhare {
namespace CalcTests {

using namespace Calc;

void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  QByteArray localMsg = msg.toLocal8Bit();
  switch (type) {
  case QtDebugMsg:
      fprintf(stderr, "DEBUG %s: %s\n",
        context.category, localMsg.constData());
      fflush(stderr);
      break;
  case QtInfoMsg:
      fprintf(stderr, "INFO %s: %s\n",
        context.category, localMsg.constData());
      fflush(stderr);
      break;
  case QtWarningMsg:
      fprintf(stderr, "WARN %s: %s\n",
        context.category, localMsg.constData());
      fflush(stderr);
      break;
  case QtCriticalMsg:
      fprintf(stderr, "CRIT %s: %s\n",
        context.category, localMsg.constData());
      fflush(stderr);
      break;
  case QtFatalMsg:
      fprintf(stderr, "FATAL %s: %s\n",
        context.category, localMsg.constData());
      fflush(stderr);
  }
}

void Parser::testOperations()
{
  qInstallMessageHandler(logHandler);

  double result;

  QVERIFY(evalExpression("1", result));
  QCOMPARE(result, 1.0);

  QVERIFY(evalExpression("1+1", result));
  QCOMPARE(result, 2.0);

  QVERIFY(evalExpression("2*3", result));
  QCOMPARE(result, 6.0);

  QVERIFY(evalExpression("2-3", result));
  QCOMPARE(result, -1.0);

  QVERIFY(evalExpression("4/2", result));
  QCOMPARE(result, 2.0);

  QVERIFY(evalExpression("- 1", result));
  QCOMPARE(result, -1.0);

  QVERIFY(evalExpression("+ 1", result));
  QCOMPARE(result, +1.0);
}

void Parser::testParentheses()
{
  qInstallMessageHandler(logHandler);

  double result;

  QVERIFY(evalExpression("1+(1+1)", result));
  QCOMPARE(result, 3.0);

  QVERIFY(evalExpression("1+(1+1)+1", result));
  QCOMPARE(result, 4.0);

  QVERIFY(evalExpression("(1+1)+1+1", result));
  QCOMPARE(result, 4.0);

  QVERIFY(evalExpression("((1+(1))+1)+1", result));
  QCOMPARE(result, 4.0);

  QVERIFY(evalExpression("(1+1)", result));
  QCOMPARE(result, 2.0);

  QVERIFY(evalExpression("(1)", result));
  QCOMPARE(result, 1.0);

  QVERIFY(evalExpression("-(1 + 1)", result));
  QCOMPARE(result, -2.0);

  QVERIFY(evalExpression("+(1 + 1)", result));
  QCOMPARE(result, 2.0);
}

void Parser::testSkipper()
{
  qInstallMessageHandler(logHandler);

  double result;

  QVERIFY(evalExpression("  1  +  (  1  +  1  )  ", result));
  QCOMPARE(result, 3.0);
}

void Parser::testOpOrder()
{
  qInstallMessageHandler(logHandler);

  double result;

  QVERIFY(evalExpression("1 + 2 * 3", result));
  QCOMPARE(result, 7.0);

  QVERIFY(evalExpression("1 - 4 / 2", result));
  QCOMPARE(result, -1.0);

  QVERIFY(evalExpression("2 - -1", result));
  QCOMPARE(result, 3.0);
}

} // namespace CalcTests
} // namespace Deskhare

QTEST_MAIN(Deskhare::CalcTests::Parser)
