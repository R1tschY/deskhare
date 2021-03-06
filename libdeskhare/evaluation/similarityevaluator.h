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

#include "evaluationservice.h"

namespace Deskhare {

/// \brief
class SimilarityEvaluator: public Evaluator
{
public:
  float eval(const Query& query, const Match& match) const override;
  bool isThreadSafe() const override { return true; }

private:
  float evalOne(const QString& querystr, const QString& keyword) const;
};

} // namespace Deskhare
