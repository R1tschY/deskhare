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

#include "iconservice.h"

#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <QIcon>
#include <QFileIconProvider>

#include "iconprovider.h"
#include <cpp-utils/assert.h>
#include <cpp-utils/algorithm/container.h>
#include <cpp-utils/lambda.h>

#include "../utils/pluginutils.h"
#include "defaulticonprovider.h"

namespace Deskhare {

IconService::IconService()
: provider_(std::make_shared<DefaultIconProvider>())
{ }

QIcon IconService::fileIcon(const QFileInfo& info)
{
  return getInstance().provider_->getFileIconProvider().icon(info);
}

QIcon IconService::iconFromTheme(const QString& name)
{
  return getInstance().provider_->iconFromTheme(name);
}

void IconService::setProvider(const std::shared_ptr<IconProvider>& provider)
{
  if (provider)
  {
    qDebug() << "Setting icon provider to"
      << getTitleFromDescription(provider->getDescription());
    getInstance().provider_ = provider;
  }
  else
  {
    qDebug() << "Resetting icon";
    getInstance().provider_ = std::make_shared<DefaultIconProvider>();
  }
}

IconService& IconService::getInstance()
{
  static IconService instance;
  return instance;
}

} // namespace Deskhare


