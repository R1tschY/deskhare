/// \file genericmatch.h

#pragma once

#include <memory>

#include "../match.h"

class QString;
class QIcon;

namespace Deskhare {

class Action;

/// \brief
class GenericMatch : public Match
{
public:
  GenericMatch(
    const QString& title,
    const QString& description,
    const QIcon& icon,
    const QString& uri,
    float score,
    std::shared_ptr<Action> default_action);

  virtual ~GenericMatch();

  std::shared_ptr<Action> getDefaultAction() const override;

private:
  std::shared_ptr<Action> default_action_;
};

} // namespace Deskhare
