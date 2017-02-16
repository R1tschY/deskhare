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

#include "action.h"
#include <libdeskhare/match.h>
#include <libdeskhare/action.h>
#include <libdeskhare/actions/openurlaction.h>

namespace Deskhare {

MatchBase::MatchBase(float score)
: score_(score)
{
}

Match::Match(const QString& uri, float score)
: MatchBase(score), uri_(uri)
{ }

std::unique_ptr<Action> Match::getDefaultAction() const
{
  return std::make_unique<OpenUrlAction>();
}

} // namespace QuickStarter


