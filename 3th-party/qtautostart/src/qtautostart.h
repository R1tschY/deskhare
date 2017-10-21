//
// qtautostart
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


class QString;

namespace QtAutostart {

/// @brief adds application to autostart
///
/// @param appName short name of application (unix name)
/// @returns Returns @c true if successful; otherwise returns @c false .
///
/// Unix:
///
/// Uses freedesktop.org specification:
///
///   https://specifications.freedesktop.org/autostart-spec/0.5/ar01s02.html
///
/// Creates @c ~/.config/autostart/<appname>.desktop file.
/// Icon entry is set to @c <appname>.
/// Name entry is set to @c QCoreApplication::applicationName().
///
void addToAutostart(const QString& appName);

/// @brief removes application from autostart
void removeFromAutostart(const QString& appName);

/// @brief detects if application is in autostart
bool isInAutostart(const QString& appName);

} // namespace QtAutostart
