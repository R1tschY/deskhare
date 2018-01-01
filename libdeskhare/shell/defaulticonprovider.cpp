/// \file defaulticonprovider.cpp

#include "defaulticonprovider.h"

namespace Deskhare {

DefaultIconProvider::DefaultIconProvider() = default;

QString DefaultIconProvider::getDescription() const
{
  return "Generic implementation for all platforms.";
}

} // namespace Deskhare
