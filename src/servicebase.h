/*
    This file is part of the KDE project

    SPDX-FileCopyrightText: 2004 Jakub Stachowski <qbast@go2.pl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KDNSSDSERVICEBASE_H
#define KDNSSDSERVICEBASE_H

#include "kdnssd_export.h"
#include <QExplicitlySharedDataPointer>
#include <QMap>
#include <QString>
#include <memory>

/*!
 * \namespace KDNSSD
 * \inmodule KDNSSD
 * \brief Interfaces to system mDNS service discovery.
 */
namespace KDNSSD
{
class ServiceBasePrivate;

/*!
 * \class KDNSSD::ServiceBase
 * \inmodule KDNSSD
 * \inheaderfile KDNSSD/ServiceBase
 *
 * \brief Describes a service.
 *
 * This class is used to describe a service. The service
 * can be published by the current application (in which
 * case it is probably a PublicService) or by
 * another application, either on the current machine or
 * a remote machine, in which case it is probably a
 * RemoteService returned by ServiceBrowser.
 *
 * \note You should not normally need to create a ServiceBase
 * object yourself.
 *
 * \sa PublicService
 */
class KDNSSD_EXPORT ServiceBase : public QSharedData // krazy:exclude=dpointer (protected)
{
public:
    typedef QExplicitlySharedDataPointer<ServiceBase> Ptr;

    /*!
     * Creates a ServiceBase object
     *
     * Note that \a name, \a type and \a domain uniquely identify
     * the service in the DNS-SD system, and \a host and \a port
     * provide the actual location of the service.
     *
     * For example, RemoteService populates \a host and \a port
     * based on the \a name, \a type and \a domain attributes
     * using the DNS-SD resolution system.
     *
     * \a name is the service name
     *
     * \a type is the service type
     *
     * \a domain is the DNS-SD domain name for service
     *
     * \a host is the host name of the service (a fully-qualified domain name)
     *
     * \a port is the port number of the service
     *
     */
    explicit ServiceBase(const QString &name = QString(),
                         const QString &type = QString(),
                         const QString &domain = QString(),
                         const QString &host = QString(),
                         unsigned short port = 0);

    virtual ~ServiceBase();

    /*!
     * Returns the name of the service.
     */
    QString serviceName() const;

    /*!
     * Returns the type of the service.
     *
     * This is always in the format \c _sometype._udp or \c _sometype._tcp.
     *
     * See \l http://www.dns-sd.org for
     * \l {http://www.dns-sd.org/ServiceTypes.html} {a full list of service types}.
     */
    QString type() const;

    /*!
     * Returns the domain that the service belongs to.
     *
     * It is "local." for link-local services.
     */
    QString domain() const;

    /*!
     * Returns the hostname of the service.
     *
     * Only valid for local and resolved remote services.
     *
     * Together with port(), this can be used to actually
     * access the service.
     *
     * \sa RemoteService::resolve() and RemoteService::resolveAsync()
     */
    QString hostName() const;

    /*!
     * Returns the port number of the service.
     *
     * Only valid for local and resolved remote services.
     *
     * Together with hostName(), this can be used to actually
     * access the service.
     *
     * \sa RemoteService::resolve() and RemoteService::resolveAsync()
     */
    unsigned short port() const;

    /*!
     * Returns additional text data associated with the service.
     *
     * Only valid for local and resolved remote services.
     *
     * This is data that provides additional information about the
     * service.  For example, it might be used to specify a printer
     * queue on the printer server specified by hostName() and port().
     *
     * You can check for the data that might be associated with a
     * particular service on the
     * \l {http://www.dns-sd.org/ServiceTypes.html} {service types list}.
     * If a \c key=value pair is given, this will appear with the \c value
     * in a QByteArray indexed by the \c key.  If the data is on its own
     * (does not have an \c = in it), it will be used to index an empty
     * QByteArray, and can be checked for with QMap::contains().
     *
     * For example, if you are accessing the _ipp._tcp service, you might
     * do something like:
     * \code
     * QString printerModel = "unknown";
     * if (service->textData().contains("ty")) {
     *     printQueue = QString::fromUtf8(service->textData()["ty"].constData());
     * }
     * \endcode
     * since the TXT data of the IPP service may contain data like
     * "ty=Apple LaserWriter Pro 630".
     *
     * \note You actually have to be
     * a bit more clever than this, since the key should usually be case
     * insensitive.
     */
    QMap<QString, QByteArray> textData() const;

    /*!
     * Compares services based on name, type and domain.
     *
     * This is enough to for unique identification and omitting
     * port, host and text data allows to compare resolved and
     * unresolved services
     *
     * \a o is the service to compare this service to
     *
     * Returns \c true if this service represents the same
     *         service (from the point of view of DNS-SD) as
     *         \a o, \c false otherwise
     */
    bool operator==(const ServiceBase &o) const;

    /*!
     * Compares services based on name, type and domain.
     *
     * This is enough to for unique identification and omitting
     * port, host and text data allows to compare resolved and
     * unresolved services
     *
     * \a o is the service to compare this service to
     *
     * Returns \c false if this service represents the same
     *         service (from the point of view of DNS-SD) as
     *         \a o, \c true otherwise
     */
    bool operator!=(const ServiceBase &o) const;

protected:
    KDNSSD_NO_EXPORT explicit ServiceBase(ServiceBasePrivate *const d);

    virtual void virtual_hook(int, void *);

protected:
    std::unique_ptr<ServiceBasePrivate> const d;
    // We cannot use Q_DECLARE_PRIVATE_D & Q_D here because of multiple inheritance with some
    // of the subclasses of ServiceBasePrivate, where ServiceBasePrivate is not the first base class,
    // so reinterpret_cast as used by the functions defined with Q_DECLARE_PRIVATE_D would fail.
    // Using a custom macro here with static_cast would require to know about the type definition
    // of the private classes, which we though want to avoid here in the public class.
    // So instead some custom KDNSSD_D macros are used internally...
};

/* Utility functions */

/*!
 * \relates KDNSSD::ServiceBase
 *
 * Check if the \a domain is link-local.
 *
 * Returns \c true if domain is link-local ('local.'), \c false otherwise
 */
bool domainIsLocal(const QString &domain);

}

#endif
