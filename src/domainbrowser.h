/*
    This file is part of the KDE project

    SPDX-FileCopyrightText: 2004 Jakub Stachowski <qbast@go2.pl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KDNSSDDOMAINBROWSER_H
#define KDNSSDDOMAINBROWSER_H

#include "remoteservice.h"
#include <QObject>
#include <QtContainerFwd>
#include <memory>

namespace KDNSSD
{
class DomainBrowserPrivate;

/*!
 * \class KDNSSD::DomainBrowser
 * \inmodule KDNSSD
 * \inheaderfile KDNSSD/DomainBrowser
 *
 * \brief Browses recommended domains for browsing or publishing to.
 *
 * Usage of this class is very simple.  If you are interested in
 * browsing for services, simple do:
 * \code
 * KDNSSD::DomainBrowser *browser =
 *     new KDNSSD::DomainBrowser(KDNSSD::DomainBrowser::Browsing, this);
 * connect(browser, SIGNAL(domainAdded(QString)),
 *         this, SLOT(browsingDomainAdded(QString));
 * connect(browser, SIGNAL(domainRemoved(QString)),
 *         this, SLOT(browsingDomainRemove(QString));
 * browser->startBrowse();
 * \endcode
 *
 * If you are interested in domains where you can register services,
 * usage is identical except that you should pass
 * KDNSSD::DomainBrowser::Publishing to the constructor.
 */
class KDNSSD_EXPORT DomainBrowser : public QObject
{
    Q_OBJECT
public:
    /*!
     * \enum KDNSSD::DomainBrowser::DomainType
     * \brief A type of domain recommendation.
     * \value Browsing Domains recommended for browsing for services on (using ServiceBrowser).
     * \value Publishing Domains recommended for publishing to (using PublicService).
     */
    enum DomainType {
        Browsing,
        Publishing,
    };
    /*!
     * Standard constructor
     *
     * The global DNS-SD configuration (for example, the global Avahi
     * configuration for the Avahi backend) will be used.
     *
     * \a type is the type of domain to search for
     *
     * \a parent is the parent object (see QObject documentation)
     *
     * \sa startBrowse() and ServiceBrowser::isAvailable()
     */
    explicit DomainBrowser(DomainType type, QObject *parent = nullptr);

    ~DomainBrowser() override;

    /*!
     * The current known list of domains of the requested DomainType.
     *
     * Returns a list of currently known domain names
     */
    QStringList domains() const;

    /*!
     * Starts browsing.
     *
     * \note Only the first call to this function will have any effect.
     *
     * Browsing stops when the DomainBrowser object is destroyed.
     *
     * \warning The domainAdded() signal may be emitted before this
     *          function returns.
     *
     * \sa domainAdded() and domainRemoved()
     */
    void startBrowse();

    /*!
     * Whether the browsing has been started.
     *
     * Returns \c true if startBrowse() has been called, \c false otherwise
     */
    bool isRunning() const;

Q_SIGNALS:
    /*!
     * A domain has disappeared from the browsed list.
     *
     * Emitted when domain has been removed from browsing list
     * or the publishing list (depending on which list was
     * requested in the constructor).
     *
     * \a domain is the name of the domain
     *
     * \sa domainAdded()
     */
    void domainRemoved(const QString &domain);

    /*!
     * A new domain has been discovered.
     *
     * If the requested DomainType is Browsing, this will
     * also be emitted for the domains specified in the
     * global configuration.
     *
     * \a domain is the name of the domain
     *
     * \sa domainRemoved()
     */
    void domainAdded(const QString &domain);

private:
    friend class DomainBrowserPrivate;
    std::unique_ptr<DomainBrowserPrivate> const d;
    Q_DECLARE_PRIVATE_D(d, DomainBrowser)
};

}

#endif
