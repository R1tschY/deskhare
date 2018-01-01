/// \file defaulticonprovider.h

#pragma once

#include "iconprovider.h"

#include <QFileIconProvider>
#include <QIcon>

namespace Deskhare {

/// \brief
class DefaultIconProvider : public IconProvider
{
public:
  DefaultIconProvider();

  float getPriorityIndex() override { return 0; }

  const QFileIconProvider& getFileIconProvider() override {
    return fileIcons_;
  }

  QIcon iconFromTheme(const QString& name) override {
    return QIcon::fromTheme(name);
  }

  QString getDescription() const override;

private:
  QFileIconProvider fileIcons_;
};

} // namespace Deskhare
