///
/// Copyright (c) 2017 R1tschY
/// 
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to 
/// deal in the Software without restriction, including without limitation the 
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.
///

#pragma once

#include <vector>
#include <QMutex>
#include <memory>

namespace Deskhare {

class Match;
class Query;
class EvaluationService;

/// \brief
class ResultSet
{
public:
  ResultSet(
    const std::shared_ptr<const Query>& query,
    const std::shared_ptr<const EvaluationService>& evaluator);

  void sendMatch(std::shared_ptr<Match> match);
  void sendMatches(std::vector<std::shared_ptr<Match>>& matches);

  void recieveMatches(std::vector<std::shared_ptr<Match>>& matches);

private:
  std::vector<std::shared_ptr<Match>> matches_buffer_;
  QMutex mutex_;
  std::shared_ptr<const Query> query_;
  std::shared_ptr<const EvaluationService> evaluation_service_;
};

} // namespace Deskhare
