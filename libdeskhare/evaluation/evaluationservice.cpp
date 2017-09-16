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

#include "evaluationservice.h"

#include <QString>
#include <cpp-utils/algorithm/range.h>

#include "query.h"
#include "match.h"
#include "history/historyservice.h"

namespace Deskhare {

EvaluationService::EvaluationService() = default;

float EvaluationService::evalWhileSend(const Query& query, const Match& match) const
{
  float score = match.getScore();
  for (auto& evaluator : ts_evaluators_)
  {
    score *= evaluator->eval(query, match);
  }
  return score;
}

float EvaluationService::evalWhileRecieve(const Query& query, const Match& match) const
{
  float score = match.getScore();
  for (auto& evaluator : evaluators_)
  {
    score *= evaluator->eval(query, match);
  }
  return score;
}

void EvaluationService::addEvaluator(
  const std::shared_ptr<const Evaluator>& evaluator)
{
  if (evaluator->isThreadSafe())
  {
    ts_evaluators_.push_back(evaluator);
  }
  else
  {
    evaluators_.push_back(evaluator);
  }
}

void EvaluationService::removeEvaluator(
  const std::shared_ptr<const Evaluator>& evaluator)
{
  auto result = cpp::range::find(ts_evaluators_, evaluator);
  ts_evaluators_.erase(result.begin(), result.end());

  result = cpp::range::find(evaluators_, evaluator);
  ts_evaluators_.erase(result.begin(), result.end());
}

// EvaluationServiceRegistry

EvaluationServiceRegistry::EvaluationServiceRegistry()
: service_(std::make_shared<EvaluationService>())
{ }

void EvaluationServiceRegistry::registerEvaluator(
  const std::shared_ptr<Evaluator>& evaluator)
{
  EvaluationService service = *service_;
  service.addEvaluator(evaluator);
  service_ = std::make_shared<EvaluationService>(std::move(service));
}


void EvaluationServiceRegistry::unregisterEvaluator(
  const std::shared_ptr<Evaluator>& evaluator)
{
  EvaluationService service = *service_;
  service.removeEvaluator(evaluator);
  service_ = std::make_shared<EvaluationService>(std::move(service));
}

} // namespace Deskhare
