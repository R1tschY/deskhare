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

#include <libdeskhare/registry.h>

#include "xdgiconprovider.h"
#include "applications/xdgapplications.h"
#include "xdginfocache.h"

namespace Deskhare {

void XdgPlugin::initialize(const PluginContext& ctx)
{
  // load cache
  getInfoCache();

  auto& registry = ctx.getRegistry();

  registry.registerSource(
    std::make_shared<XdgApplications>(ctx));

  registry.registerIconProvider(
    std::make_shared<XdgIconProvider>());
}

QString XdgPlugin::getDescription()
{
  return QLatin1String("integration for freedesktop compatible unix systems");
}

} // namespace Deskhare
