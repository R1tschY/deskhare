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

#include <memory>
#include <QVector>

namespace Deskhare {

class Query;
class Match;
class HistoryService;

class Evaluator
{
public:
  virtual ~Evaluator() = default;

  virtual float eval(const Query& query, const Match& match) const = 0;
  virtual bool isThreadSafe() const = 0;
};


/// \brief sets score how good match fits to query
class EvaluationService
{
public:
  EvaluationService();

  float evalWhileSend(const Query& query, const Match& match) const;
  float evalWhileRecieve(const Query& query, const Match& match) const;

  void addEvaluator(const std::shared_ptr<const Evaluator>& evaluator);
  void removeEvaluator(const std::shared_ptr<const Evaluator>& evaluator);

private:
  QVector<std::shared_ptr<const Evaluator>> ts_evaluators_;
  QVector<std::shared_ptr<const Evaluator>> evaluators_;
};

class EvaluationServiceRegistry
{
public:
  EvaluationServiceRegistry();

  void registerEvaluator(const std::shared_ptr<Evaluator>& evaluator);
  void unregisterEvaluator(const std::shared_ptr<Evaluator>& evaluator);

  std::shared_ptr<const EvaluationService> getEvaluationService() const
  {
    return service_;
  }

private:
  std::shared_ptr<const EvaluationService> service_;
};

} // namespace Deskhare
