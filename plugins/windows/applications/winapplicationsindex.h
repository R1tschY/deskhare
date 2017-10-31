/// \file winapplicationsindex.h

#pragma once

#include <memory>
#include <vector>

#include <libdeskhare/indexes/sqliteindex.h>
#include <winqt/shell/iconlocation.h>

namespace Deskhare {

class Match;
class Action;

/// \brief
class WinApplicationsIndex : public SqliteIndex
{
public:
  WinApplicationsIndex();
  virtual ~WinApplicationsIndex();

  void clear();

  void addApplication(const QString& name, const QString& description,
    const QString& path, const WinQt::IconLocation& iconLoc);

  std::vector<std::shared_ptr<Match>> search(const QString& query);

  QDateTime getLastIndexing();
  void setLastIndexing(const QDateTime& time);

private:
  bool create() override;
  bool upgrade(int currentFormatVersion) override;
};

} // namespace Deskhare
