//
// xdg-thumbnails-qt
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

#include <QUrl>
#include <QImage>

class XdgThumbnail : public QImage
{
public:
  /// Image size category of thumbnail
  enum ImageSize {
    Normal, //!< Normal size (128px x 128px)
    Large   //!< Large size (256px x 256px)
  };

  explicit XdgThumbnail() = default;

  explicit XdgThumbnail(const QString& fileName)
  : QImage(fileName)
  { }
};

/// Thumbnail management class
class XdgThumbnails
{
  XdgThumbnails() = default;
public:

  /// Get file path to thumbnail file (may not exist).
  ///
  /// No lookup is done wheter the file exists or not. For example: if a large
  /// size was requested and a large thumbnail does not exist, but a normal size
  /// thumbnail does. It will not return a path to a thumbnail of
  /// normal size.
  ///
  /// @see getThumbnail
  ///
  /// @param fileUrl Url to file of which the thumbnail is requested.
  ///   Should not be a local relative file path. Is will generate an undesired
  ///   result. The file path should be normalized (no '.', '..' or empty path
  ///   segments)
  /// @param size Wheter to return path to large or normal sized image.
  /// @return file path (empty if @p fileUrl is invalid).
  static QString getThumbnailFile(
    const QUrl& fileUrl, XdgThumbnail::ImageSize size = XdgThumbnail::Large);

  /// Get thumbnail image.
  ///
  /// Maybe empty if no thumbnail does exist (with the requested size).
  ///
  /// @see getThumbnailFile
  ///
  /// @param fileUrl Url to file of which the thumbnail is requested.
  ///   Should not be a local relative file path. Is will generate an undesired
  ///   result. The file path should be normalized (no '.', '..' or empty path
  ///   segments)
  /// @param size Wheter to return image to large or normal sized image.
  /// @return file path (empty if @p fileUrl is invalid).
  static XdgThumbnail getThumbnail(
    const QUrl& fileUrl, XdgThumbnail::ImageSize size = XdgThumbnail::Large)
  {
    return XdgThumbnail(getThumbnailFile(fileUrl, size));
  }
};
