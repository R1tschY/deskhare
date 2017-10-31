//
// deskhare - cross-platform quick launcher
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

#include "shelllink.h"

#include <shobjidl.h>
#include <shlguid.h>
#include <shobjidl.h>
#include <cassert>
#include <QString>
#include <QKeySequence>
#include <QIcon>

#include "../common/bits.h"
#include "../common/qthelper.h"
#include "../common/comobject.h"
#include "iconlocation.h"

namespace WinQt {

struct ShellLinkPrivate
{
  // TODO: ComContext
  Idl_<IShellLinkW> shellLink;
  Result lastResult = Result(S_OK);
};


ShellLink::ShellLink()
: d_(new ShellLinkPrivate())
{
  Result res = d_->shellLink.create(
    CLSID_ShellLink,
    CLSCTX_INPROC_SERVER,
    IID_IShellLink
  );
  if (!res)
  {
    d_->lastResult = res;
    return;
  }
}

ShellLink::~ShellLink() = default;

Result ShellLink::load(const QString& fileName)
{
  if (!okay()) return d_->lastResult;

  Result res;

  Idl_<IPersistFile> file;
  res = d_->shellLink.queryInterface(file);
  if (!res)
  {
    d_->lastResult = res;
    return res;
  }

  res = file->Load(winStringView(fileName), STGM_READ);
  if (!res)
  {
    d_->lastResult = res;
    return res;
  }

  return res;
}

QString ShellLink::description() const
{
  if (!okay()) return QString();

  wchar_t result[INFOTIPSIZE]; // TODO: better solution
  d_->lastResult = d_->shellLink->GetDescription(result, ARRAYSIZE(result));

  if (d_->lastResult)
    return QString::fromWCharArray(result);
  else
    return QString();
}

QString ShellLink::path() const
{
  if (!okay()) return QString();

  wchar_t result[MAX_PATH];
  d_->lastResult = d_->shellLink->GetPath(
    result, ARRAYSIZE(result), nullptr, 0);
  if (d_->lastResult == S_OK)
    return QString::fromWCharArray(result);
  else
    return QString();
}

IconLocation ShellLink::iconLocation() const
{
  if (!okay()) return IconLocation();

  wchar_t path[MAX_PATH];
  int index;
  d_->lastResult = d_->shellLink->GetIconLocation(
    path, ARRAYSIZE(path), &index);
  if (d_->lastResult)
    return IconLocation(QString::fromWCharArray(path), index);
  else
    return IconLocation();
}

bool ShellLink::okay() const
{
  return !d_->shellLink.empty();
}

QIcon ShellLink::icon() const
{
  return iconLocation().resolve();
}

Result ShellLink::lastResult()
{
  return d_->lastResult;
}

//
//QKeySequence ShellLink::getHotkey() const
//{
//  uint16_t win_result;
//  setLastResult(
//    get()->GetHotkey(&win_result)
//  );
//
//  int keyseq = low_byte(win_result);
//  uint8_t mod = high_byte(win_result);
//  if (mod & HOTKEYF_SHIFT)   keyseq += Qt::SHIFT;
//  if (mod & HOTKEYF_CONTROL) keyseq += Qt::CTRL;
//  if (mod & HOTKEYF_ALT)     keyseq += Qt::ALT;
//  if (mod & HOTKEYF_EXT)     keyseq += Qt::META;
//  return QKeySequence(keyseq);
//}

} /* namespace WinQt */
