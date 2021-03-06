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

#include "resultset.h"

#include "evaluation/evaluationservice.h"
#include "match.h"

namespace Deskhare {

ResultSet::ResultSet(
  const std::shared_ptr<const Query>& query,
  const std::shared_ptr<const EvaluationService>& evaluator
)
: query_(query), evaluation_service_(evaluator)
{ }

void ResultSet::sendMatch(std::shared_ptr<Match> match)
{
  match->setScore(evaluation_service_->evalWhileSend(*query_, *match));

  QMutexLocker lock(&mutex_);

  matches_buffer_.push_back(std::move(match));
}

void ResultSet::sendMatches(std::vector<std::shared_ptr<Match>>& matches)
{
  for (auto& match : matches)
  {
    match->setScore(evaluation_service_->evalWhileSend(*query_, *match));
  }

  QMutexLocker lock(&mutex_);

  matches_buffer_.insert(matches_buffer_.end(),
    std::make_move_iterator(matches.begin()),
    std::make_move_iterator(matches.end()));
  matches.clear();
}

void ResultSet::sendMatches(std::vector<std::shared_ptr<Match>>&& matches)
{
  for (auto& match : matches)
  {
    match->setScore(evaluation_service_->evalWhileSend(*query_, *match));
  }

  QMutexLocker lock(&mutex_);

  matches_buffer_.insert(matches_buffer_.end(),
    std::make_move_iterator(matches.begin()),
    std::make_move_iterator(matches.end()));
}

void ResultSet::recieveMatches(std::vector<std::shared_ptr<Match>>& matches)
{
  QMutexLocker lock(&mutex_);

  matches.insert(matches.end(),
    std::make_move_iterator(matches_buffer_.begin()),
    std::make_move_iterator(matches_buffer_.end()));
  matches_buffer_.clear();

  lock.unlock();

  for (auto& match : matches)
  {
    match->setScore(evaluation_service_->evalWhileRecieve(*query_, *match));
  }
}

} // namespace Deskhare
