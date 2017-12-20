//
// deskhare
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

#include "plugin.h"

#include <libdeskhare/registry.h>

#include "calculatorsource.h"


namespace Deskhare {

void CalcPlugin::initialize(const PluginContext& ctx)
{
  auto& registry = ctx.getRegistry();

  registry.registerSource(
    std::make_shared<CalculatorSource>(ctx));
}

QString CalcPlugin::getDescription()
{
  return QLatin1String("a calculator");
}

} // namespace Deskhare

