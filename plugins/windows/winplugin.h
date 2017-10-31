/// \file winplugin.h

#pragma once

#include <libdeskhare/sourceplugin.h>
#include <winqt/common/comcontext.h>

namespace Deskhare {

/// \brief
class WinPlugin :
  public QObject,
  public SourcePlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DeskhareSource_iid)
  Q_INTERFACES(Deskhare::SourcePlugin)
public:
  WinPlugin();

  // SourcePlugin
  std::unique_ptr<Source> getSource(const PluginContext& ctx) override;
  QString getSourceDescription() override;

private:
  WinQt::ComContext context_;
};

} // namespace Deskhare

