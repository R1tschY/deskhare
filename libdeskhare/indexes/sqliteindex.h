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

#pragma once

#include <QSqlDatabase>
#include <QObject>
#include <cpp-utils/scope.h>

namespace Deskhare {

/// \brief
class SqliteIndex
{
public:
  SqliteIndex(const QString& indexName, int formatVersion);
  virtual ~SqliteIndex() = default;

  bool open(const QString& filePath);

  QSqlDatabase& dataBase() { return db_; }
  bool isOpen() const { return db_.isOpen(); }

  auto createTransaction() __attribute__((warn_unused_result))
  {
    db_.transaction();
    return cpp::make_transaction(
      [&](){ db_.commit(); },
      [&](){ db_.rollback(); }
    );
  }

  auto createTransactionGuard() __attribute__((warn_unused_result))
  {
    db_.transaction();
    return cpp::make_transaction_guard(
      [&](){ db_.commit(); },
      [&](){ db_.rollback(); }
    );
  }

  static QString escape(QString string);
  static QString escape(QChar c);
  static QString escapeLike(QChar c, QChar escapeChar);

  static QString createIndexPath(const QString& fileName);

  void printDbDriverFeatures();

private:
  virtual bool create() = 0;
  virtual bool upgrade(int currentFormatVersion) = 0;

  int getCurrentFormatVersion();
  bool setCurrentFormatVersion();
  bool recreate(bool useFallback = false);
  bool doUpgrade(int currentFormatVersion);
  bool doCreate();
  bool isEmpty() const;

private:
  QSqlDatabase db_;
  int formatVersion_ = 0;
};

} // namespace Deskhare
