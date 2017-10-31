/// \file genericmatch.cpp

#include "genericmatch.h"

namespace Deskhare {

GenericMatch::GenericMatch(
  const QString& title,
  const QString& description,
  const QIcon& icon,
  const QString& uri,
  float score,
  std::shared_ptr<Action> default_action)
: Match(title, description, icon, uri, score), default_action_(default_action)
{ }

GenericMatch::~GenericMatch() = default;

std::shared_ptr<Action> GenericMatch::getDefaultAction() const
{
  return default_action_;
}

} // namespace Deskhare
