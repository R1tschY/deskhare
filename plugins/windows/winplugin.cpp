#include "winplugin.h"

#include <libdeskhare/registry.h>

#include "applications/startmenusource.h"
#include "icons/winiconprovider.h"

namespace Deskhare {

WinPlugin::WinPlugin()
// for COM and ShellExecuteEx
: context_(COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)
{ }

void WinPlugin::initialize(const PluginContext& ctx)
{
  auto& registry = ctx.getRegistry();

  registry.registerSource(
    std::make_shared<StartMenuSource>(ctx));
  registry.registerIconProvider(
    std::make_shared<WinIconProvider>());
}

QString WinPlugin::getDescription()
{
  return QLatin1String("Windows integration.\n"
    "\n"
    "Access to applications.");
}

} // namespace Deskhare
