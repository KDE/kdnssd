/* This file is part of the KDE project
 *
 * Copyright (C) 2008 Jakub Stachowski <qbast@go2.pl>
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

#ifndef KDNSSDDOMAINMODEL_H
#define KDNSSDDOMAINMODEL_H

#include <QtCore/QAbstractItemModel>
#include <dnssd/kdnssd_export.h>

namespace KDNSSD
{

struct DomainModelPrivate;
class DomainBrowser;

/**
 * @class DomainModel domainmodel.h KDNSSD/DomainModel
 * @short Model for list of Zeroconf domains
 *
 * This class provides a Qt Model for DomainBrowser to allow easy
 * integration of domain discovery into a GUI.  For example, to
 * provide a combo box listing available domains, you can do:
 * @code
 * KDNSSD::DomainModel *domainModel = new DomainModel(
 *     new KDNSSD::DomainBrowser(KDNSSD::DomainBrowser::Browsing)
 *     );
 * QComboBox *domainCombo = new QComboBox();
 * domainCombo->setModel(domainModel);
 * @endcode
 *
 * @since 4.1
 * @author Jakub Stachowski
 */

class KDNSSD_EXPORT DomainModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    /**
     * Creates a model for given domain browser and starts
     * browsing for domains.
     *
     * The model takes ownership of the browser,
     * so there is no need to delete it afterwards.
     *
     * You should @b not call DomainBrowser::startBrowse() on @p browser
     * before passing it to DomainModel.
     *
     * @param browser the domain browser that will provide the domains
     *                to be listed by the model
     * @param parent  the parent object (see QObject documentation)
     */
    explicit DomainModel(DomainBrowser *browser, QObject *parent = nullptr);

    virtual ~DomainModel();

    /** @reimp */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    /** @reimp */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    /** @reimp */
    QModelIndex parent(const QModelIndex &index) const override;
    /** @reimp */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    /** @reimp */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    /** @reimp */
    virtual bool hasIndex(int row, int column, const QModelIndex &parent) const;

private:
    DomainModelPrivate *const d;
    friend struct DomainModelPrivate;

};

}

#endif
