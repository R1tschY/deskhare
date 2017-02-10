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

#include "../../../../libdeskhare/platforms/windows/shell/shelllink.h"

#include <shobjidl.h>
#include <shlguid.h>
#include <shobjidl.h>
#include <cassert>
#include <c++/utils.h>
#include <QString>
#include <QKeySequence>

#include "../../../../libdeskhare/platforms/windows/bits.h"
#include "../../../../libdeskhare/platforms/windows/debug.h"

namespace Windows {

ShellLink::ShellLink()
{
  // create
  IShellLinkW* shell_link = nullptr;
  win_throw_on_fail(
    CoCreateInstance(
      CLSID_ShellLink,
      nullptr,
      CLSCTX_INPROC_SERVER,
      IID_IShellLink,
      reinterpret_cast<void**>(&shell_link)
    )
  );
  shell_link_.reset(shell_link);

  IPersistFile* tmp;
  win_throw_on_fail(
    shell_link_->QueryInterface(
      IID_IPersistFile,
      reinterpret_cast<void**>(&tmp)
    )
  );
  persist_file.reset(tmp);
}

ShellLink::~ShellLink() { }

void ShellLink::open(const QString& path, unsigned mode)
{
  // open file
  win_throw_on_fail(
    persist_file->Load((const wchar_t*)path.utf16(), mode)
  );
}

void ShellLink::resolve(unsigned timeout_ms, unsigned flags)
{
  assert(shell_link_ != nullptr);

  high_word(flags, timeout_ms);

  win_throw_on_fail(
    shell_link_->Resolve(nullptr, SLR_NO_UI | flags)
  );
}

void ShellLink::resolve(HWND window, unsigned flags)
{
  assert(shell_link_ != nullptr);

  win_throw_on_fail(
    shell_link_->Resolve(window, flags)
  );
}

void ShellLink::getArguments(QString& value) const
{
  assert(shell_link_ != nullptr);

  wchar_t dest[MAX_PATH];
  win_throw_on_fail(
    shell_link_->GetArguments(dest, cpp::length(dest))
  );
  value.setUtf16((ushort*)dest, wcslen(dest));
}

void ShellLink::getDescription(QString& value) const
{
  wchar_t dest[MAX_PATH];
  win_throw_on_fail(
    shell_link_->GetDescription(dest, cpp::length(dest))
  );
  value.setUtf16((ushort*)dest, wcslen(dest));
}

QKeySequence ShellLink::getHotkey() const
{
  uint16_t win_result;
  win_throw_on_fail(
    shell_link_->GetHotkey(&win_result)
  );

  int keyseq = low_byte(win_result);
  uint8_t mod = high_byte(win_result);
  if (mod & HOTKEYF_SHIFT)   keyseq += Qt::SHIFT;
  if (mod & HOTKEYF_CONTROL) keyseq += Qt::CTRL;
  if (mod & HOTKEYF_ALT)     keyseq += Qt::ALT;
  if (mod & HOTKEYF_EXT)     keyseq += Qt::META;
  return QKeySequence(keyseq);
}

void ShellLink::getIconLocation(QString& path, int& index) const
{
  wchar_t dest[MAX_PATH];
  win_throw_on_fail(
    shell_link_->GetIconLocation(dest, cpp::length(dest), &index)
  );
  path.setUtf16((ushort*)dest, wcslen(dest));
}

void ShellLink::getPath(QString& value, unsigned flags) const
{
  wchar_t dest[MAX_PATH];
  win_throw_on_fail(
    shell_link_->GetPath(dest, cpp::length(dest), nullptr, flags)
  );
  value.setUtf16((ushort*)dest, wcslen(dest));
}

void ShellLink::getShowCmd(int& value) const
{
  win_throw_on_fail(
    shell_link_->GetShowCmd(&value)
  );
}

void ShellLink::getWorkingDirectory(QString& value) const
{
  wchar_t dest[MAX_PATH];
  win_throw_on_fail(
    shell_link_->GetWorkingDirectory(dest, cpp::length(dest))
  );
  value.setUtf16((ushort*)dest, wcslen(dest));
}

} /* namespace Windows */
