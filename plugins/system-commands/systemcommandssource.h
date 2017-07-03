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

#pragma once

#include <libdeskhare/source.h>
#include <libdeskhare/plugincontext.h>
#include <libdeskhare/match.h>

namespace Deskhare {

class PluginContext;

/// \brief
class SystemCommandsSource : public Source
{
public:
  SystemCommandsSource(const PluginContext& ctx);
  ~SystemCommandsSource();

  bool canHandleQuery(const Query& query) override;
  void search(const Query& query, ResultSet& results) override;

private:
  PluginContext ctx_;
  MatchResults commands_;
};

} // namespace Deskhare
