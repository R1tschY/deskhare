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

#include "query.h"
#include "match.h"
#include "history/historyservice.h"

namespace Deskhare {

EvaluationService::EvaluationService(HistoryService& history)
: history_(history)
{ }

float EvaluationService::evalWhileSend(const Query& query, const Match& match)
{
  // everything must be thread-safe

  QString querystr = query.getSearchString();
  QString matched = match.getTitle();

  float init = match.getScore();

  if (querystr.length() == 0 || matched.length() == 0)
    // no evaluation possible
    return 1.0 * init;

  if (matched.startsWith(querystr, Qt::CaseInsensitive))
  {
    return 1.0 * init;
  }
  else if (matched.indexOf(querystr, Qt::CaseInsensitive) != -1)
  {
    return 0.95 * init;
  }
  else if (matched[0].toLower() == querystr[0].toLower())
  {
    return 0.75 * init;
  }
  else
  {
    return 0.5 * init;
  }
}

float EvaluationService::evalWhileRecieve(const Query& query, const Match& match)
{
  return match.getScore() + history_.eval(query, match);
}

} // namespace Deskhare
