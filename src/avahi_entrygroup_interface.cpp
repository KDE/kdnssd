/*
 * This file was generated by dbusxml2cpp version 0.6
 * Command line was: dbusxml2cpp -m -p avahi_entrygroup_interface /home/qba/src/kdelibs/dnssd/org.freedesktop.Avahi.EntryGroup.xml
 *
 * dbusxml2cpp is Copyright (C) 2006 Trolltech ASA. All rights reserved.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "avahi_entrygroup_interface.h"
/*
 * Implementation of interface class OrgFreedesktopAvahiEntryGroupInterface
 */

OrgFreedesktopAvahiEntryGroupInterface::OrgFreedesktopAvahiEntryGroupInterface(const QString &service,
                                                                               const QString &path,
                                                                               const QDBusConnection &connection,
                                                                               QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

OrgFreedesktopAvahiEntryGroupInterface::~OrgFreedesktopAvahiEntryGroupInterface()
{
}

#include "moc_avahi_entrygroup_interface.cpp"
