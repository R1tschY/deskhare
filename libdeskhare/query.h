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

#ifndef APP_PLUGINS_QUICKSTARTER_SOURCES_QUERY_H_
#define APP_PLUGINS_QUICKSTARTER_SOURCES_QUERY_H_

#include <QString>
#include <atomic>

class QRegularExpression;

namespace Deskhare {

class Match;

/// \brief
class Query
{
public:
  enum class Categories {
    Action   = 1 << 0,
    App      = 1 << 1,
    Location = 1 << 2,
    Web      = 1 << 3,
    File     = 1 << 4,

    All      = 0xFFFF
  };

  Query(Categories categories, const QString& search_string,
    const Match* target = nullptr);

  Categories getCategories() const { return categories_; }

  bool hasCategory(Categories value) const
  {
    return static_cast<int>(categories_) & static_cast<int>(value);
  }

  const QString& getSearchString() const { return search_string_; }

  void cancel() { canceled_ = true; }
  bool canceled() const volatile { return canceled_; }

  const Match* getTarget() const { return target_; }

  QRegularExpression getSearchRegex() const;

private:
  const Categories categories_;
  const QString search_string_;
  std::atomic<bool> canceled_;
  const Match* target_;
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_SOURCES_QUERY_H_ */
