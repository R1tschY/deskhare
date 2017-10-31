/// \file winapplicationaction.cpp

#include "winapplicationaction.h"

#include <QDesktopServices>
#include <QUrl>
#include <winqt/common/qthelper.h>

namespace Deskhare {

using namespace WinQt;

WinApplicationAction::WinApplicationAction(const QString& path, float score)
: Action(
  tr("Execute"),
  tr("Execute application"),
  QIcon(), // TODO
  score),
  path_(path)
{ }

bool WinApplicationAction::canHandleMatch(const Match& match) const
{
  return true;
}

void WinApplicationAction::execute(const Match& target) const
{
  QDesktopServices::openUrl(QUrl::fromLocalFile(path_));
}

} // namespace Deskhare
