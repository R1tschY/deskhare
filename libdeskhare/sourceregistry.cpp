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

#include "sourceregistry.h"

#include <QLoggingCategory>
#include <cpp-utils/algorithm/container.h>

#include "source.h"
#include "utils/pluginutils.h"

namespace Deskhare {

static Q_LOGGING_CATEGORY(logger, "deskhare.sourceregistry")

SourceRegistry::~SourceRegistry() = default;

void SourceRegistry::registerSource(const std::shared_ptr<Source>& source)
{
  if (cpp::has(sources_, source))
  {
    qCWarning(logger)
      << "Source double registered:"
      << getTitleFromDescription(source->getDescription());
    return;
  }

  qCInfo(logger).noquote() << "got source:"
    << getTitleFromDescription(source->getDescription());

  sources_.emplace_back(source);
}

void SourceRegistry::registerActionSource(const std::shared_ptr<Source>& source)
{
  if (cpp::has(action_sources_, source))
  {
    qCWarning(logger)
      << "Action source double registered:"
      << getTitleFromDescription(source->getDescription());
    return;
  }

  qCInfo(logger).noquote()
    << "got action source:"
    << getTitleFromDescription(source->getDescription());

  action_sources_.emplace_back(source);
}


void SourceRegistry::registerSourceShell(
  const QString& queryPrefix,
  const std::shared_ptr<Source>& source)
{
  if (cpp::any_of(source_shells_,
        [&](const auto& e) { return std::get<0>(e) == queryPrefix; }))
  {
    qCWarning(logger)
      << "Shell prefix" << queryPrefix << "is already registered:"
      << getTitleFromDescription(source->getDescription());
    return;
  }

  qCInfo(logger).noquote() << "got source shell:"
    << getTitleFromDescription(source->getDescription());

  source_shells_.emplace_back(std::make_tuple(queryPrefix, source));
}

void SourceRegistry::registerIconProvider(
  const std::shared_ptr<IconProvider>& provider)
{
  qCInfo(logger).noquote() << "got file icon provider:"
    << getTitleFromDescription(provider->getDescription());

  icon_providers_.emplace(provider);
}

QVector<std::shared_ptr<Source>> SourceRegistry::getSources() const
{
  return QVector<std::shared_ptr<Source>>::fromStdVector(sources_);
}

QVector<std::shared_ptr<Source>> SourceRegistry::getActionSources() const
{
  return QVector<std::shared_ptr<Source>>::fromStdVector(action_sources_);
}

std::shared_ptr<Source> SourceRegistry::findSourceShell(
  const QString& query) const
{
  return {};
}

std::shared_ptr<IconProvider> SourceRegistry::getBestFileIconProvider() const
{
  return (!icon_providers_.empty())
    ? icon_providers_.top()
    : std::shared_ptr<IconProvider>();
}

} // namespace Deskhare
