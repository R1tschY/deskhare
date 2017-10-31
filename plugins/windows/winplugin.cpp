#include "winplugin.h"

#include "applications/startmenusource.h"

namespace Deskhare {

WinPlugin::WinPlugin()
// for COM and ShellExecuteEx
: context_(COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)
{ }

std::unique_ptr<Source> WinPlugin::getSource(const PluginContext& ctx)
{
  return std::make_unique<StartMenuSource>(ctx);
}

QString WinPlugin::getSourceDescription()
{
  return QLatin1String("Windows applications.\n"
    "\n"
    "Access to applications in the start menu and on the desktop.");
}

} // namespace Deskhare
