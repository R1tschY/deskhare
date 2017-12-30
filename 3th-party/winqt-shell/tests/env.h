#pragma once

#include <QObject>

namespace WinQt {

/// \brief
class EnvTest : public QObject
{
  Q_OBJECT
private slots:

  void testExpandEnvironmentStrings();
};

} // namespace WinQt
