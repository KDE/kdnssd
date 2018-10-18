/* This file is part of the KDE project
 *
 * Copyright (C) 2004,2007 Jakub Stachowski <qbast@go2.pl>
 * Copyright (C) 2018 Harald Sitter <sitter@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "avahi-servicetypebrowser_p.h"
#include <QSet>
#include "avahi_server_interface.h"
#include "servicetypebrowser.h"
#include "avahi_servicetypebrowser_interface.h"

#define UNSPEC -1
namespace KDNSSD
{

ServiceTypeBrowser::ServiceTypeBrowser(const QString &domain, QObject *parent) : QObject(parent), d(new ServiceTypeBrowserPrivate(this))
{
    d->m_domain = domain;
    d->m_timer.setSingleShot(true);
}

ServiceTypeBrowser::~ServiceTypeBrowser()
{
    delete d;
}

void ServiceTypeBrowser::startBrowse()
{
    if (d->m_started) {
        return;
    }
    d->m_started = true;

    // Do not race!
    // https://github.com/lathiat/avahi/issues/9
    // Avahi's DBus API is incredibly racey with signals getting fired
    // immediately after a request was made even though we may not yet be
    // listening. In lieu of a proper upstream fix for this we'll unfortunately
    // have to resort to this hack:
    // We register to all signals regardless of path and then filter them once
    // we know what "our" path is. This is much more fragile than a proper
    // QDBusInterface assisted signal connection but unfortunately the only way
    // we can reliably prevent signals getting lost in the race.
    // This uses a fancy trick whereby using QDBusMessage as last argument will
    // give us the correct signal argument types as well as the underlying
    // message so that we may check the message path.
    QDBusConnection::systemBus()
            .connect("org.freedesktop.Avahi",
                     "",
                     "org.freedesktop.Avahi.ServiceTypeBrowser",
                     "ItemNew",
                     d,
                     SLOT(gotGlobalItemNew(int,int,QString,QString,uint,QDBusMessage)));
    QDBusConnection::systemBus()
            .connect("org.freedesktop.Avahi",
                     "",
                     "org.freedesktop.Avahi.ServiceTypeBrowser",
                     "ItemRemove",
                     d,
                     SLOT(gotGlobalItemRemove(int,int,QString,QString,uint,QDBusMessage)));
    QDBusConnection::systemBus()
            .connect("org.freedesktop.Avahi",
                     "",
                     "org.freedesktop.Avahi.ServiceTypeBrowser",
                     "AllForNow",
                     d,
                     SLOT(gotGlobalAllForNow(QDBusMessage)));
    d->m_dbusObjectPath.clear();

    org::freedesktop::Avahi::Server s(QStringLiteral("org.freedesktop.Avahi"), QStringLiteral("/"), QDBusConnection::systemBus());

    QDBusReply<QDBusObjectPath> rep = s.ServiceTypeBrowserNew(-1, -1, d->m_domain, 0);
    if (!rep.isValid()) {
        return;
    }

    d->m_dbusObjectPath = rep.value().path();

    // This is held because we need to explicitly Free it!
    d->m_browser = new org::freedesktop::Avahi::ServiceTypeBrowser(
                s.service(),
                d->m_dbusObjectPath,
                s.connection());

    connect(&d->m_timer, SIGNAL(timeout()), d, SLOT(finished()));
    d->m_timer.start(domainIsLocal(d->m_domain) ? TIMEOUT_LAST_SERVICE : TIMEOUT_START_WAN);
}

void ServiceTypeBrowserPrivate::finished()
{
    m_timer.stop();
    emit m_parent->finished();
}

void ServiceTypeBrowserPrivate::gotGlobalItemNew(int interface,
                                                 int protocol,
                                                 const QString &type,
                                                 const QString &domain,
                                                 uint flags,
                                                 QDBusMessage msg)
{
    if (!isOurMsg(msg)) {
        return;
    }
    gotNewServiceType(interface, protocol, type, domain, flags);
}

void ServiceTypeBrowserPrivate::gotGlobalItemRemove(int interface,
                                                    int protocol,
                                                    const QString &type,
                                                    const QString &domain,
                                                    uint flags,
                                                    QDBusMessage msg)
{
    if (!isOurMsg(msg)) {
        return;
    }
    gotRemoveServiceType(interface, protocol, type, domain, flags);
}

void ServiceTypeBrowserPrivate::gotGlobalAllForNow(QDBusMessage msg)
{
    if (!isOurMsg(msg)) {
        return;
    }
    finished();
}

void ServiceTypeBrowserPrivate::gotNewServiceType(int, int, const QString &type, const QString &, uint)
{
    m_timer.start(TIMEOUT_LAST_SERVICE);
    m_servicetypes += type;
    emit m_parent->serviceTypeAdded(type);
}

void ServiceTypeBrowserPrivate::gotRemoveServiceType(int, int, const QString &type, const QString &, uint)
{
    m_timer.start(TIMEOUT_LAST_SERVICE);
    m_servicetypes.removeAll(type);
    emit m_parent->serviceTypeRemoved(type);
}

QStringList ServiceTypeBrowser::serviceTypes() const
{
    return d->m_servicetypes;
}

}
#include "moc_servicetypebrowser.cpp"
#include "moc_avahi-servicetypebrowser_p.cpp"
