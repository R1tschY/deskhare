/// \file winplugin.h

#pragma once

#include <QObject>

#include <libdeskhare/plugin.h>
#include <winqt/common/comcontext.h>

namespace Deskhare {

/// \brief
class WinPlugin : public QObject, public Plugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DeskharePlugin_iid)
  Q_INTERFACES(Deskhare::Plugin)
public:
  WinPlugin();

  void initialize(const PluginContext& ctx) override;
  QString getDescription() override;

private:
  WinQt::ComContext context_;
};

} // namespace Deskhare

