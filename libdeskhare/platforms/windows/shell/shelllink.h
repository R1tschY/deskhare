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

#ifndef HOOKLIB_WINDOWS_COM_SHELLLINK_H_
#define HOOKLIB_WINDOWS_COM_SHELLLINK_H_

#include <windows.h>

#include "../../../../libdeskhare/platforms/windows/com/comcontext.h"
#include "../../../../libdeskhare/platforms/windows/com/comptr.h"

class IShellLinkW;
class IPersistFile;
class QString;
class QKeySequence;

namespace Windows {

class ShellLink
{
public:
  ShellLink();
  ~ShellLink();

  ShellLink(const ShellLink&) = delete;
  ShellLink& operator=(const ShellLink&) = delete;

  // IPersistFile::Load
  void open(const QString& path, unsigned mode = 0 /* STGM_READ */);

  // IShellLink::Resolve
  void resolve(unsigned timeout_ms = 3000, unsigned flags = 0);
  void resolve(HWND window, unsigned flags);

  // IShellLink::GetArguments
  void getArguments(QString& value) const;

  // IShellLink::GetDescription
  void getDescription(QString& value) const;

  // IShellLink::GetHotkey
  QKeySequence getHotkey() const;

  // IShellLink::GetIconLocation
  void getIconLocation(QString& path, int& index) const;

  // IShellLink::GetPath
  void getPath(QString& value, unsigned flags = 0) const;

  // IShellLink::GetShowCmd
  void getShowCmd(int& value) const;

  // IShellLink::GetWorkingDirectory
  void getWorkingDirectory(QString& value) const;

  // internals
  IShellLinkW* getIShellLink() const { return shell_link_.get(); }

private:
  COMPtr<IShellLinkW> shell_link_;
  COMPtr<IPersistFile> persist_file;
  //COMContext context_;
};

} /* namespace Windows */

#endif /* HOOKLIB_WINDOWS_COM_SHELLLINK_H_ */
