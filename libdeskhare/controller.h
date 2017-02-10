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

#ifndef APP_PLUGINS_QUICKSTARTER_CONTROLLER_H_
#define APP_PLUGINS_QUICKSTARTER_CONTROLLER_H_

#include <vector>
#include <memory>

#include <libdeskhare/pluginmanager.h>
#include <libdeskhare/source.h>
#include <libdeskhare/shell/fileiconservice.h>

class QString;

namespace Deskhare {

class Source;
class Action;

class Controller
{
public:
  Controller();
  ~Controller();

  MatchResults search(const QString& query) const;
  bool execute(const Match& match) const;

private:
  std::vector<std::unique_ptr<Source>> sources_;
  FileIconProvider file_icon_provider_;
  PluginManager plugin_manager_;
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_CONTROLLER_H_ */
