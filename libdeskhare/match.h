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

#ifndef APP_PLUGINS_QUICKSTARTER_SOURCES_MATCH_H_
#define APP_PLUGINS_QUICKSTARTER_SOURCES_MATCH_H_

#include <string>
#include <utility>
#include <vector>
#include <QString>
#include <QUrl>
#include <QIcon>
#include <memory>

namespace Deskhare {

class Action;

namespace MatchScore {
enum MatchScore
{
  IncrementMinor = 2000,
  IncrementSmall = 5000,
  IncrementMedium = 10000,
  IncrementLarge = 20000,

  Poor = 50000,
  BelowAverage = 60000,
  Average = 70000,
  AboveAverage = 75000,
  Good = 80000,
  VeryGood = 85000,
  Excellent = 90000,

  Highest = 100000
};
}

/// \brief Base class for match and action
class MatchBase
{
public:
  MatchBase(float score = 0);
  virtual ~MatchBase() = default;

  virtual QString getDescription() const = 0;
  virtual QString getTitle() const = 0;
  virtual QIcon getIcon() const = 0;

  float getScore() const { return score_; }
  void setScore(float score) { score_ = score; }

private:
  float score_;
};

class Match : public MatchBase
{
public:
  using MatchBase::MatchBase;

  Match(const QString& uri, float score = 0);

  virtual std::unique_ptr<Action> getDefaultAction() const;

  QString getUri() const { return uri_; }

private:
  const QString uri_;
};

using MatchResults = std::vector<std::unique_ptr<Match>>;

struct MatchScoreComparer
{
  bool operator()(const std::unique_ptr<Match>& lhs, const std::unique_ptr<Match>& rhs) const
  {
    return lhs->getScore() > rhs->getScore();
  }
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_SOURCES_MATCH_H_ */
