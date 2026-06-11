/*
    SPDX-FileCopyrightText: 2026 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <KDNSSD/DomainBrowser>
#include <KDNSSD/DomainModel>
#include <KDNSSD/RemoteService>
#include <KDNSSD/ServiceBrowser>
#include <KDNSSD/ServiceModel>
#include <KDNSSD/ServiceTypeBrowser>

#include <QApplication>
#include <QComboBox>
#include <QListView>
#include <QTreeView>
#include <QVBoxLayout>

using namespace Qt::Literals;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QWidget mainWidget;
    auto topLayout = new QVBoxLayout(&mainWidget);

    auto typeBox = new QComboBox;
    topLayout->addWidget(typeBox);

    auto serviceTypeBrowser = new KDNSSD::ServiceTypeBrowser({}, &app);
    serviceTypeBrowser->startBrowse();
    QObject::connect(serviceTypeBrowser, &KDNSSD::ServiceTypeBrowser::serviceTypeAdded, typeBox, [&]() {
        typeBox->clear();
        typeBox->addItems(serviceTypeBrowser->serviceTypes());
    });
    QObject::connect(serviceTypeBrowser, &KDNSSD::ServiceTypeBrowser::serviceTypeRemoved, typeBox, [&]() {
        typeBox->clear();
        typeBox->addItems(serviceTypeBrowser->serviceTypes());
    });
    typeBox->addItems(serviceTypeBrowser->serviceTypes());

    auto serviceView = new QTreeView;
    serviceView->setRootIsDecorated(false);
    serviceView->setSelectionBehavior(QTreeView::SelectRows);
    topLayout->addWidget(serviceView);

    KDNSSD::ServiceModel *serviceModel = nullptr;
    QObject::connect(typeBox, &QComboBox::currentTextChanged, &app, [&](const QString &type) {
        delete serviceModel;
        auto serviceModel = new KDNSSD::ServiceModel(new KDNSSD::ServiceBrowser(type, true));
        serviceView->setModel(serviceModel);
        QObject::connect(serviceView->selectionModel(), &QItemSelectionModel::currentRowChanged, &app, [](const auto &idx) {
            if (idx.isValid()) {
                auto service = idx.data(KDNSSD::ServiceModel::ServicePtrRole).template value<KDNSSD::RemoteService::Ptr>();
                qDebug() << service->textData();
            }
        });
    });

    mainWidget.show();
    return app.exec();
}
