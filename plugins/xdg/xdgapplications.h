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

#pragma once

#include <libdeskhare/source.h>

#include "xdgapplicationdesktopfile.h"

namespace Deskhare {

/// \brief
class XdgApplications : public Source
{
public:
  XdgApplications(const PluginContext& ctx);

private:
  bool canHandleQuery(const Query& query) override;
  void search(const Query& query, ResultSet& results) override;

  void index();

  struct IndexEntry
  {
    IndexEntry(const XdgApplicationDesktopFile& desktopFile);

    QString title;
    QString genericName;
    QString keywords;
    QString appname;
    XdgApplicationDesktopFile desktopFile;
    float score = 0;
  };
  std::vector<IndexEntry> index_;
  std::vector<QString> whitelist_;
};

} // namespace Deskhare
