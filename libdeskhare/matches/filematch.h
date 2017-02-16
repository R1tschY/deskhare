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

#include <qfileinfo.h>
#include <qstring.h>

#include "../match.h"

namespace Deskhare {

class FileIconProvider;

class LocalFileMatch : public Match
{
public:
  LocalFileMatch(const QString& filePath, const FileIconProvider& icons);

  QString getDescription() const override;
  QString getTitle() const override;
  QIcon getIcon() const override;

private:
  QFileInfo fileInfo_;

  QString description_;
  QIcon icon_;
};

} // namespace Deskhare
