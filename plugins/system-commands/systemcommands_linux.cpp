//
// deskhare
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

#include <atomic>
#include <QByteArray>
#include <QLoggingCategory>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusReply>
#include <QtConcurrent>
#include <cpp-utils/down_cast.h>

#include <libdeskhare/action.h>
#include <libdeskhare/match.h>

#include "plugin.h"

namespace Deskhare {

namespace {

Q_LOGGING_CATEGORY(logger, "deskhare.system-commands.linux");

const QString kSystemdService = QLatin1String("org.freedesktop.login1");
const QString kSystemdManagerObjectPath
  = QLatin1String("/org/freedesktop/login1");
const QString kSystemdManagerInterface
  = QLatin1String("org.freedesktop.login1.Manager");

const QString kSystemdSessionObjectPath
  = QLatin1String("/org/freedesktop/login1/session/self");
const QString kSystemdSessionInterface
  = QLatin1String("org.freedesktop.login1.Session");

bool isSystemdRunning()
{
  QDBusInterface systemdPeer(kSystemdService, "/",
    "org.freedesktop.DBus.Peer", QDBusConnection::systemBus());

  QDBusReply<void> reply = systemdPeer.call("Ping");
  return reply.isValid();
}

class SystemAction : public Action
{
public:
  using Action::Action;

  bool canHandleMatch(const Match& match) const
  {
    return &match == this;
  }

  virtual void checkAvaliability() = 0;
  bool getAvaliability() const { return avaliable_; }

protected:
  void setAvaliability(bool value) { avaliable_ = value; }

  static QString dbusErrorString(const QDBusError& error)
  {
    return QDBusError::errorString(error.type()) + ": " + error.message();
  }

private:
  std::atomic<bool> avaliable_{false};
};

class PowerOffAction : public SystemAction
{
public:
  PowerOffAction()
  : SystemAction(
    SystemCommandsPlugin::tr("Shut Down"),
    SystemCommandsPlugin::tr("Turn computer off"),
    QIcon::fromTheme("system-shutdown"),
    QLatin1String("system-command://poweroff"),
    MatchScore::Good
  )
  { }

  void execute(const Match& target) const
  {
    QDBusInterface systemdManager(kSystemdService, kSystemdManagerObjectPath,
      kSystemdManagerInterface, QDBusConnection::systemBus());

    QDBusReply<void> reply = systemdManager.call("PowerOff", true);
    if (!reply.isValid())
    {
      qCCritical(logger) << "power off failed:" <<
        dbusErrorString(reply.error());
    }
  }

  void checkAvaliability() override
  {
    QDBusInterface systemdManager(kSystemdService, kSystemdManagerObjectPath,
      kSystemdManagerInterface, QDBusConnection::systemBus());

    QDBusReply<QString> reply = systemdManager.call("CanPowerOff");
    if (!reply.isValid())
    {
      qCCritical(logger) << "check for power off failed:" <<
        dbusErrorString(reply.error());
      return;
    }

    setAvaliability(reply.value() != QLatin1String("na"));
  }
};

class RestartAction : public SystemAction
{
public:
  RestartAction()
  : SystemAction(
    SystemCommandsPlugin::tr("Restart"),
    SystemCommandsPlugin::tr("Restart computer"),
    QIcon::fromTheme("gnome-session-reboot"),
    QLatin1String("system-command://reboot"),
    MatchScore::Good
  )
  { }

  void execute(const Match& target) const
  {
    QDBusInterface systemdManager(kSystemdService, kSystemdManagerObjectPath,
      kSystemdManagerInterface, QDBusConnection::systemBus());

    QDBusReply<void> reply = systemdManager.call("Reboot", true);
    if (!reply.isValid())
    {
      qCCritical(logger) << "reboot failed:" <<
        dbusErrorString(reply.error());
    }
  }

  void checkAvaliability() override
  {
    QDBusInterface systemdManager(kSystemdService, kSystemdManagerObjectPath,
      kSystemdManagerInterface, QDBusConnection::systemBus());

    QDBusReply<QString> reply = systemdManager.call("CanReboot");
    if (!reply.isValid())
    {
      qCCritical(logger) << "check for reboot failed:" <<
        dbusErrorString(reply.error());
      return;
    }

    setAvaliability(reply.value() != QLatin1String("na"));
  }
};

class HibernateAction : public SystemAction
{
public:
  HibernateAction()
  : SystemAction(
    SystemCommandsPlugin::tr("Hibernate"),
    SystemCommandsPlugin::tr("Put computer into hibernation mode"),
    QIcon::fromTheme("system-hibernate"),
    QLatin1String("system-command://hibernate"),
    MatchScore::Good
  )
  { }

  void execute(const Match& target) const
  {
    QDBusInterface systemdManager(kSystemdService, kSystemdManagerObjectPath,
      kSystemdManagerInterface, QDBusConnection::systemBus());

    QDBusReply<void> reply = systemdManager.call("Hibernate", true);
    if (!reply.isValid())
    {
      qCCritical(logger) << "hibernate failed:" <<
        dbusErrorString(reply.error());
      return;
    }
  }

  void checkAvaliability() override
  {
    QDBusInterface systemdManager(kSystemdService, kSystemdManagerObjectPath,
      kSystemdManagerInterface, QDBusConnection::systemBus());

    QDBusReply<QString> reply = systemdManager.call("CanHibernate");
    if (!reply.isValid())
    {
      qCCritical(logger) << "check for hibernate failed:" <<
        dbusErrorString(reply.error());
      return;
    }

    setAvaliability(reply.value() != QLatin1String("na"));
  }
};

class SuspendAction : public SystemAction
{
public:
  SuspendAction()
  : SystemAction(
    SystemCommandsPlugin::tr("Suspend"),
    SystemCommandsPlugin::tr("Put computer into suspend mode"),
    QIcon::fromTheme("system-suspend"),
    QLatin1String("system-command://suspend"),
    MatchScore::Good
  )
  { }

  void execute(const Match& target) const
  {
    QDBusInterface systemdManager(kSystemdService, kSystemdManagerObjectPath,
      kSystemdManagerInterface, QDBusConnection::systemBus());

    QDBusReply<void> reply = systemdManager.call("Suspend", true);
    if (!reply.isValid())
    {
      qCCritical(logger) << "supend failed:" <<
        dbusErrorString(reply.error());
    }
  }

  void checkAvaliability() override
  {
    QDBusInterface systemdManager(kSystemdService, kSystemdManagerObjectPath,
      kSystemdManagerInterface, QDBusConnection::systemBus());

    QDBusReply<QString> reply = systemdManager.call("CanSuspend");
    if (!reply.isValid())
    {
      qCCritical(logger) << "check for suspend failed:" <<
        dbusErrorString(reply.error());
      return;
    }

    setAvaliability(reply.value() != QLatin1String("na"));
  }
};

class LogoutAction : public SystemAction
{
public:
  LogoutAction()
  : SystemAction(
    SystemCommandsPlugin::tr("Logout"),
    SystemCommandsPlugin::tr("Logout current user"),
    QIcon::fromTheme("gnome-session-logout"),
    QLatin1String("system-command://logout"),
    MatchScore::Good
  )
  { }

  void execute(const Match& target) const
  {
    QDBusInterface systemdManager(kSystemdService, kSystemdSessionObjectPath,
      kSystemdSessionInterface, QDBusConnection::systemBus());

    QDBusReply<void> reply = systemdManager.call("Terminate", true);
    if (!reply.isValid())
    {
      qCCritical(logger) << "logout failed:" <<
        dbusErrorString(reply.error());
    }
  }

  void checkAvaliability() override
  {
    setAvaliability(true);
  }
};

class LockDesktopAction : public SystemAction
{
public:
  LockDesktopAction()
  : SystemAction(
    SystemCommandsPlugin::tr("Lock"),
    SystemCommandsPlugin::tr("Lock screen"),
    QIcon::fromTheme("system-lock-screen"),
    QLatin1String("system-command://lock"),
    MatchScore::Good
  )
  { }

  void execute(const Match& target) const
  {
    QDBusInterface systemdManager(kSystemdService, kSystemdSessionObjectPath,
      kSystemdSessionInterface, QDBusConnection::systemBus());

    QDBusReply<void> reply = systemdManager.call("Lock");
    if (!reply.isValid())
    {
      qCCritical(logger) << "lock desktop failed:" <<
        dbusErrorString(reply.error());
    }
  }

  void checkAvaliability() override
  {
    setAvaliability(true);
  }
};

} // namespace

MatchResults getSystemCommands()
{
  MatchResults systemCommands;

  if (!isSystemdRunning())
  {
    qCWarning(logger) << "cannot detect running systemd: "
      "no support for linux systems without systemd";
    return systemCommands;
  }

  systemCommands.push_back(std::make_shared<PowerOffAction>());
  systemCommands.push_back(std::make_shared<RestartAction>());
  systemCommands.push_back(std::make_shared<HibernateAction>());
  systemCommands.push_back(std::make_shared<SuspendAction>());
  systemCommands.push_back(std::make_shared<LogoutAction>());
  systemCommands.push_back(std::make_shared<LockDesktopAction>());

  QtConcurrent::run([=](){
    for (const auto& systemCommand : systemCommands)
    {
      cpp::down_cast<SystemAction>(systemCommand.get())->checkAvaliability();
    }
  });

  return systemCommands;
}

} // namespace Deskhare
