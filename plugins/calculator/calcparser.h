//
// deskhare
// Copyright (C) 2017 Richard Liebscher
//
// This program is free software: you can redistribute it and/or modify
// it under the Terms of the GNU General Public License as published by
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

#include <QString>
#include <cpp-utils/compiler/lib-export.h>

#ifdef BUILD_CALC_PARSER
#define CALC_PARSER_EXPORT CPP_LIB_EXPORT
#else
#define CALC_PARSER_EXPORT CPP_LIB_IMPORT
#endif

namespace Deskhare {
namespace Calc {

CALC_PARSER_EXPORT
bool evalExpression(const QString& input, double& result);

} // namespace Calc
} // namespace Deskhare
