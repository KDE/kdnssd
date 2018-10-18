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

#ifndef AVAHI_DOMAINBROWSER_P_H
#define AVAHI_DOMAINBROWSER_P_H

#include <QSet>
#include "domainbrowser.h"
#include "avahi_listener_p.h"
#include "avahi_domainbrowser_interface.h"

namespace KDNSSD
{

class DomainBrowserPrivate : public QObject, public AvahiListener
{
    Q_OBJECT
public:
    DomainBrowserPrivate(DomainBrowser::DomainType type, DomainBrowser *parent) : m_type(type), m_browser(nullptr), m_parent(parent), m_started(false) {}
    ~DomainBrowserPrivate()
    {
        if (m_browser) {
            m_browser->Free();
        }
    }

    DomainBrowser::DomainType m_type;
    org::freedesktop::Avahi::DomainBrowser *m_browser = nullptr;
    DomainBrowser *m_parent = nullptr;
    bool m_started = false;
    QSet<QString> m_domains;

public Q_SLOTS:
    // NB: The global slots are runtime connected! If their signature changes
    // make sure the SLOT() signature gets updated!
    void gotGlobalItemNew(int interface,
                          int protocol,
                          const QString &domain,
                          uint flags,
                          QDBusMessage msg);
    void gotGlobalItemRemove(int interface,
                             int protocol,
                             const QString &domain,
                             uint flags,
                             QDBusMessage msg);
    void gotGlobalAllForNow(QDBusMessage msg);

    void gotNewDomain(int, int, const QString &, uint);
    void gotRemoveDomain(int, int, const QString &, uint);
};

}
#endif
