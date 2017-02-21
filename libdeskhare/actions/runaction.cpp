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

#include "runaction.h"

namespace Deskhare {

//RunAction::RunAction()
//: Action()
//{
//
//}
//
//bool RunAction::canHandleMatch(const Match& match) const
//{
//  return true;
//}
//
//void RunAction::execute(const Match& match) const
//{/*
//  if (match.hasInfo(Match::Application))
//  {
//    Match::InfoData data = match.getInfo(Match::Application);
//
//    // search working dir
//    auto iter = std::find_if(data.begin(), data.end(), cpp::first_equal(L"working directory"));
//    cpp::wstring_view working_directory =
//      (iter != data.end()) ? cpp::wstring_view(iter->second) : cpp::wstring_view();
//
//    // TODO show command
//
//    iter = std::find_if(data.begin(), data.end(), cpp::first_equal(L"command line"));
//    if (iter == data.end())
//    {
//      throw std::runtime_error("needed data in match lacks");
//    }
//
//    Windows::Process::runCmdln(cpp::wstring_view(iter->second), working_directory);
//  }
//  else if (match.hasInfo(Match::Action))
//  {
//    match.execute();
//  }*/
//}

} // namespace QuickStarter
