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

#include "xdgplugin.h"

#include "xdgfileiconprovider.h"
#include "xdgapplications.h"

namespace Deskhare {

XdgPlugin::XdgPlugin()
{
  // TODO Auto-generated constructor stub

}

float XdgPlugin::getFileIconProviderPriorityIndex()
{
  return 10;
}

std::unique_ptr<QFileIconProvider> XdgPlugin::getFileIconProvider()
{
  return std::make_unique<XdgFileIconProvider>();
}

QString XdgPlugin::getFileIconProviderDescription() const
{
  return "Xdg file icon provider.\n"
    "\n"
    "File icon provider for Freedesktop compatible systems.";
}

std::unique_ptr<Source> XdgPlugin::getSource(const PluginContext& ctx)
{
  return std::make_unique<XdgApplications>(ctx);
}

QString XdgPlugin::getSourceDescription()
{
  return "Xdg applications.\n"
    "\n"
    "Search for applications in Freedesktop compatible systems.";
}

} // namespace Deskhare
