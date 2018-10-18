/* This file is part of the KDE project
 *
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

#ifndef AVAHILISTENER_H
#define AVAHILISTENER_H

#include <QDBusMessage>
#include <QString>

namespace KDNSSD {

// Assists with listening to Avahi for all signals and then checking if the
// a given dbus message is meant for us or not.
// Subclass and set the object path to object path you should be listening to.
// Messages may then be run through isOurMsg to determine if they target the
// object at hand.
class AvahiListener
{
public:
    explicit AvahiListener();
    virtual ~AvahiListener();

    // This method gets called a lot but doesn't do much. Suggest inlining.
    inline bool isOurMsg(const QDBusMessage &msg) const
    {
        if (m_dbusObjectPath.isEmpty() || m_dbusObjectPath != msg.path()) {
            return false;
        }
        return true;
    }

    QString m_dbusObjectPath; // public so !Private objects can access it
};

} // namespace KDNSSD

#endif // AVAHILISTENER_H
