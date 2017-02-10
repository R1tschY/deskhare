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

#include "fileiconservice.h"

#include <QDebug>
#include <cpp-utils/assert.h>
#include <cpp-utils/algorithm/container.h>
#include <cpp-utils/lambda.h>

#include "fileiconproviderplugin.h"
#include "../utils/pluginutils.h"

namespace Deskhare {

FileIconProvider::FileIconProvider()
: provider_(std::make_unique<QFileIconProvider>())
{ }

QIcon FileIconProvider::icon(QFileIconProvider::IconType type) const
{
  return provider_->icon(type);
}

QIcon FileIconProvider::icon(const QFileInfo& info) const
{
  return provider_->icon(info);
}

QString FileIconProvider::type(const QFileInfo& info) const
{
  return provider_->type(info);
}

void FileIconProvider::setProvider(std::unique_ptr<QFileIconProvider> provider)
{
  if (provider == nullptr)
  {
    provider_ = std::make_unique<QFileIconProvider>();
  }
  else
  {
    provider_ = std::move(provider);
  }
}

void FileIconProvider::updateFromPlugins(
  const std::vector<FileIconProviderPlugin*>& plugins)
{
  auto iter = cpp::max_element_by(plugins,
    lambda_(_->getFileIconProviderPriorityIndex()));

  if (iter != plugins.end())
  {
    qDebug() << "Setting file icon provider to"
      << getTitleFromDescription((*iter)->getFileIconProviderDescription());
    setProvider((*iter)->getFileIconProvider());
  }
  else
  {
    setProvider(nullptr);
  }
}

} // namespace Deskhare


