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

#include <clocale>
#include <QString>

class QLocale;

namespace Deskhare {

/// \brief
class LinuxLocale
{
public:
  LinuxLocale()
  : LinuxLocale(LC_ALL)
  { }

  LinuxLocale(int category);
  LinuxLocale(const QString& name);

  QString language() const { return language_; }
  void setLanguage(QString language) { language_ = language; }

  const QString& country() const { return country_; }
  void setCountry(const QString& country) { country_ = country; }

  QString encoding() const { return encoding_; }
  void setEncoding(const QString& encoding) { encoding_ = encoding; }

  QString modifier() const { return modifier_; }
  void setModifier(const QString& modifier) { modifier_ = modifier; }

  QLocale toLocale() const;
  QString toString() const;

  bool operator==(const LinuxLocale& other) const;

private:
  QString language_;
  QString modifier_;
  QString encoding_;
  QString country_;
};

} // namespace Deskhare
