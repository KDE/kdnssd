/*
    This file is part of the KDE project

    SPDX-FileCopyrightText: 2008 Jakub Stachowski <qbast@go2.pl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KDNSSDSERVICEMODEL_H
#define KDNSSDSERVICEMODEL_H

#include "kdnssd_export.h"
#include "remoteservice.h"
#include <QAbstractItemModel>
#include <memory>

namespace KDNSSD
{
struct ServiceModelPrivate;
class ServiceBrowser;

/*!
 * \class KDNSSD::ServiceModel
 * \inmodule KDNSSD
 * \inheaderfile KDNSSD/ServiceModel
 *
 * \brief Model for list of Zeroconf services.
 *
 * This class provides a Qt Model for ServiceBrowser to allow easy
 * integration of service discovery into a GUI.  For example, to
 * show the HTTP servers published on the local network, you can do:
 * \code
 * KDNSSD::ServiceModel *serviceModel = new ServiceModel(
 *     new KDNSSD::ServiceBrowser("_http._tcp")
 *     );
 * QComboBox *serviceCombo = new QComboBox();
 * serviceCombo->setModel(serviceModel);
 * \endcode
 *
 * After the user makes a selection, the application typically needs
 * to get a pointer to the selected service in order to get the host
 * name and port.  A RemoteService::Ptr can be obtained from
 * a QModelIndex using:
 * \code
 * void onSelected(const QModelIndex &selection) {
 *     KDNSSD::RemoteService::Ptr service =
 *         selection.data(KDNSSD::ServiceModel::ServicePtrRole)
 *                  .value<KDNSSD::RemoteService::Ptr>();
 * }
 * \endcode
 *
 * \since 4.1
 */
class KDNSSD_EXPORT ServiceModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    /*! \enum KDNSSD::ServiceModel::AdditionalRoles
     * \brief The additional data roles provided by this model.
     *
     * \value ServicePtrRole
     * Gets a RemoteService::Ptr for the service.
     */
    enum AdditionalRoles {
        ServicePtrRole = 0x7E6519DE,
    };

    /*!
     * \enum KDNSSD::ServiceModel::ModelColumns
     * \brief The default columns for this model.
     *
     * If service browser is not set to resolve automatically,
     * then the model will only ever have one column (the service name).
     *
     * \value ServiceName
     * The name of this service.
     * \value Host
     * The hostname or IP address the service is hosted on.
     * \value Port
     * The TCP or UDP port the service is hosted on.
     */
    enum ModelColumns {
        ServiceName = 0,
        Host = 1,
        Port = 2,
    };

    /*!
     * Creates a model for the given service browser and starts browsing
     * for services.
     *
     * \note
     * The model takes ownership of the browser,
     * so there is no need to delete it afterwards.
     *
     * \note You should \b not call ServiceBrowser::startBrowse() on \a browser
     * before passing it to ServiceModel.
     */
    explicit ServiceModel(ServiceBrowser *browser, QObject *parent = nullptr);

    ~ServiceModel() override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /*!  */
    virtual bool hasIndex(int row, int column, const QModelIndex &parent) const;

private:
    std::unique_ptr<ServiceModelPrivate> const d;
    friend struct ServiceModelPrivate;
};

}

#endif
