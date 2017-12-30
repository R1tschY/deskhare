/// \file logging.cpp

#include "logging.h"

#include <QtGlobal>
#include <QDebug>
#ifdef Q_OS_WIN32
#include <windows.h>
#endif
#include <cstdio>

namespace Deskhare {

bool isConsoleRedirected()
{
#ifdef Q_OS_WIN32
  HANDLE stdoutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
  if (stdoutHandle != INVALID_HANDLE_VALUE && stdoutHandle != nullptr)
  {
    DWORD filetype = ::GetFileType(stdoutHandle);
    return filetype == FILE_TYPE_CHAR || filetype == FILE_TYPE_PIPE;
  }
  return false;
#else
  return true;
#endif
}

void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  QByteArray localMsg = msg.toLocal8Bit();
  switch (type) {
  case QtDebugMsg:
      fprintf(stderr, "\x1b[37m[DEBUG] %s: %s\x1b[0m\n",
        context.category, localMsg.constData());
      fflush(stderr);
      break;
  case QtInfoMsg:
      fprintf(stderr, "\x1b[0m[INFO]\x1b[0m %s: %s\x1b[0m\n",
        context.category, localMsg.constData());
      fflush(stderr);
      break;
  case QtWarningMsg:
      fprintf(stderr, "\x1b[33m[WARN]\x1b[0m %s: %s\n",
        context.category, localMsg.constData());
      fflush(stderr);
      break;
  case QtCriticalMsg:
      fprintf(stderr, "\x1b[31m[CRIT]\x1b[0m %s: %s\x1b[0m\n",
        context.category, localMsg.constData());
      fflush(stderr);
      break;
  case QtFatalMsg:
      fprintf(stderr, "\x1b[31m[FATAL]\x1b[0m %s: %s\x1b[0m\n",
        context.category, localMsg.constData());
      abort();
  }
}

void initLogging()
{
  if (isConsoleRedirected())
  {
    // TODO: check for color support
    qInstallMessageHandler(logHandler);
  }
  else
  {
    qSetMessagePattern(QStringLiteral(
      "[%{if-debug}DEBUG%{endif}"
      "%{if-info}INFO%{endif}"
      "%{if-warning}WARN%{endif}"
      "%{if-critical}CRIT%{endif}"
      "%{if-fatal}FATAL%{endif}] "
      "%{if-category}%{category}: %{endif}"
      "%{message}"));
  }
}

} // namespace Deskhare

