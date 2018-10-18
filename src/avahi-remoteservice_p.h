/* This file is part of the KDE project
 *
 * Copyright (C) 2004 Jakub Stachowski <qbast@go2.pl>
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

#ifndef AVAHI_REMOTESERVICE_P_H
#define AVAHI_REMOTESERVICE_P_H

#include <QString>
#include <QList>
#include <QMap>
#include "servicebase_p.h"
#include "remoteservice.h"
#include "avahi_listener_p.h"
#include "avahi_serviceresolver_interface.h"

#define K_D RemoteServicePrivate* d=static_cast<RemoteServicePrivate*>(this->d)

namespace KDNSSD
{

class RemoteServicePrivate : public QObject, public ServiceBasePrivate, public AvahiListener
{
    Q_OBJECT
public:
    RemoteServicePrivate(RemoteService *parent, const QString &name, const QString &type, const QString &domain) : QObject(),
        ServiceBasePrivate(name, type, domain, QString(), 0), m_resolved(false), m_running(false), m_resolver(nullptr), m_parent(parent)
    {}
    ~RemoteServicePrivate()
    {
        if (m_resolver) {
            m_resolver->Free();
        } delete m_resolver;
    }
    bool m_resolved = false;
    bool m_running = false;
    org::freedesktop::Avahi::ServiceResolver *m_resolver = nullptr;
    RemoteService *m_parent = nullptr;
    void stop();

private Q_SLOTS:
    // NB: The global slots are runtime connected! If their signature changes
    // make sure the SLOT() signature gets updated!
    void gotGlobalFound(
            int interface,
            int protocol,
            const QString &name,
            const QString &type,
            const QString &domain,
            const QString &host,
            int aprotocol,
            const QString &address,
            ushort port,
            const QList<QByteArray> &txt,
            uint flags,
            QDBusMessage msg);
    void gotGlobalError(QDBusMessage msg);

    void gotFound(
            int interface,
            int protocol,
            const QString &name,
            const QString &type,
            const QString &domain,
            const QString &host,
            int aprotocol,
            const QString &address,
            ushort port,
            const QList<QByteArray> &txt,
            uint flags);
    void gotError();
};

}

#endif
