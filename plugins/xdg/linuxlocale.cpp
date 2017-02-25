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

#include "linuxlocale.h"

#include <QLocale>

namespace Deskhare {

LinuxLocale::LinuxLocale(int category)
: LinuxLocale(QString::fromLocal8Bit(std::setlocale(category, nullptr)))
{ }

LinuxLocale::LinuxLocale(const QString& name)
{
  language_ = name;

  modifier_ = language_.section('@', 1);
  if (!modifier_.isEmpty())
    language_.truncate(language_.length() - modifier_.length() - 1);

  encoding_ = language_.section('.', 1);
  if (!encoding_.isEmpty())
    language_.truncate(language_.length() - encoding_.length() - 1);

  country_ = language_.section('_', 1);
  if (!country_.isEmpty())
    language_.truncate(language_.length() - country_.length() - 1);
}

QLocale LinuxLocale::toLocale() const
{
  QString result = language_;
  if (result.isEmpty())
    return QLocale(result);

  if (!country_.isEmpty())
  {
    result.append('_');
    result.append(country_);
  }

  return QLocale(result);
}

QString LinuxLocale::toString() const
{
  QString result = language_;
  if (result.isEmpty())
    return result;

  if (!country_.isEmpty())
  {
    result.append('_');
    result.append(country_);
  }

  if (!encoding_.isEmpty())
  {
    result.append('.');
    result.append(encoding_);
  }

  if (!modifier_.isEmpty())
  {
    result.append('@');
    result.append(modifier_);
  }

  return result;
}

} // namespace Deskhare
