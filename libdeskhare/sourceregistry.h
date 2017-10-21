//
// deskhare
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

#include <vector>
#include <memory>
#include <tuple>
#include <queue>
#include <QString>

#include "registry.h"
#include "shell/fileiconproviderplugin.h"

namespace Deskhare {

class Source;

struct FileIconProviderCompare
{
  bool operator()(
    const std::shared_ptr<FileIconProviderPlugin>& lhs,
    const std::shared_ptr<FileIconProviderPlugin>& rhs)
  {
    return
      lhs->getFileIconProviderPriorityIndex()
      < rhs->getFileIconProviderPriorityIndex();
  }
};

class SourceRegistry : public Registry {
public:
  ~SourceRegistry();

  void registerSource(const std::shared_ptr<Source>& source) override;
  void registerActionSource(const std::shared_ptr<Source>& source) override;
  void registerSourceShell(
    const QString& queryPrefix, const std::shared_ptr<Source>& source) override;
  void registerFileIconProvider(
      const std::shared_ptr<FileIconProviderPlugin>& provider) override;

  QVector<std::shared_ptr<Source>> getSources() const;
  QVector<std::shared_ptr<Source>> getActionSources() const;

  std::shared_ptr<Source> findSourceShell(const QString& query) const;
  std::shared_ptr<FileIconProviderPlugin> getBestFileIconProvider() const;

private:
  std::vector<std::shared_ptr<Source>> sources_;
  std::vector<std::shared_ptr<Source>> action_sources_;
  std::priority_queue<
    std::shared_ptr<FileIconProviderPlugin>,
    std::vector<std::shared_ptr<FileIconProviderPlugin>>,
    FileIconProviderCompare>
      icon_providers_;
  std::vector<std::tuple<QString, std::shared_ptr<Source>>> source_shells_;
};

} // namespace Deskhare
