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

#include "xdgapplications.h"

#include <cpp-utils/as_const.h>
#include <cpp-utils/lambda.h>
#include <cpp-utils/algorithm/container.h>
#include <xdgdesktopfile.h>
#include <QFileInfo>
#include <QDebug>
#include <QRegularExpression>
#include <libdeskhare/query.h>
#include <libdeskhare/resultset.h>

#include "xdgapplicationmatch.h"
#include "xdgcommon.h"

namespace Deskhare {

XdgApplications::XdgApplications(const PluginContext& ctx)
{
  index();
}

bool XdgApplications::canHandleQuery(const Query& query)
{
  return query.hasCategory(Query::Category::App)
    && !query.getSearchString().isEmpty();
}

void XdgApplications::search(const Query& query, ResultSet& results)
{
  auto regex = query.getSearchRegex();

  std::vector<std::shared_ptr<Match>> matches;
  for (auto& entry : cpp::as_const(index_))
  {
    float score = 0;
    if (regex.match(entry.appname).hasMatch()
      || regex.match(entry.title).hasMatch())
    {
      score = entry.score;
    }
    else if (regex.match(entry.keywords).hasMatch())
    {
      score = entry.score - MatchScore::IncrementMedium;
    }
    else
    {
      continue;
    }

    matches.emplace_back(new XdgApplicationMatch(entry.desktopFile, score));
  }
  results.sendMatches(matches);
}

void XdgApplications::index()
{
  qCDebug(xdgLogger) << "start indexing ...";

  std::size_t n = 0;
  QList<XdgDesktopFile*> desktopFiles = XdgDesktopFileCache::getAllFiles();

  index_.clear();
  index_.reserve(desktopFiles.size());

  for (XdgDesktopFile* desktopFile : cpp::as_const(desktopFiles))
  {
    n += 1;
    if (desktopFile->isSuitable())
      index_.emplace_back(*desktopFile);
  }

  qCDebug(xdgLogger) << "indexed" << n << "applications";
}

XdgApplications::IndexEntry::IndexEntry(
  const XdgApplicationDesktopFile& xdgDesktopFile)
{
  desktopFile = xdgDesktopFile;
  QFileInfo fileInfo(desktopFile.fileName());

  appname = fileInfo.completeBaseName();

  title = desktopFile.localizedValue(QStringLiteral("Name")).toString();

  genericName = desktopFile.localizedValue(
    QStringLiteral("GenericName")).toString();

  keywords = desktopFile.localizedValue(
    QStringLiteral("Keywords")).toString();

  if (!desktopFile.isSuitable(false))
  {
    score = MatchScore::Poor;
  }
  else if (desktopFile.value(QStringLiteral("NoDisplay")).toBool())
  {
    score = MatchScore::Average;
  }
  else
  {
    score = MatchScore::Excellent;
  }
}

} // namespace Deskhare
