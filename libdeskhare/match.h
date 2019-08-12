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

/// \brief
class Match : public std::enable_shared_from_this<Match>
{
public:
  Match(
    const QString& title,
    const QString& description,
    const QIcon& icon,
    const QString& uri,
    float score);

  Match(
    const QString& title,
    const QString& description,
    const QIcon& icon,
    const QString& uri,
    const QStringList& keywords_,
    float score);

  virtual ~Match() = default;

  QString getDescription() const { return description_; };
  QString getTitle() const { return title_; };
  QIcon getIcon() const { return icon_; };
  QString getUri() const { return uri_; }
  QStringList getKeywords() const { return keywords_; };

  float getScore() const { return score_; }
  void setScore(float score) { score_ = score; }


  virtual std::shared_ptr<Action> getDefaultAction() const = 0;

private:
  float score_;
  const QString uri_;
  const QString description_;
  const QString title_;
  const QIcon icon_;
  const QStringList keywords_;
};

using MatchResults = std::vector<std::shared_ptr<Match>>;

struct MatchScoreComparer
{
  bool operator()(const std::shared_ptr<Match>& lhs, const std::shared_ptr<Match>& rhs) const
  {
    return lhs->getScore() > rhs->getScore();
  }
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_SOURCES_MATCH_H_ */
