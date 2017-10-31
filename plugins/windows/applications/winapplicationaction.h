#pragma once

#include <QString>
#include <QCoreApplication>

#include <libdeskhare/action.h>

namespace Deskhare {

/// \brief
class WinApplicationAction: public Action
{
  Q_DECLARE_TR_FUNCTIONS(WinApplicationAction)
public:
  WinApplicationAction(const QString& path, float score);

  bool canHandleMatch(const Match& match) const override;
  void execute(const Match& target) const override;

private:
  QString path_;
};

} // namespace Deskhare
