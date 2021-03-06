/*

PingYou

Copyright (C) 2017 Sam Hobbs

This file is part of PingYou.

PingYou is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

PingYou is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with PingYou.  If not, see <http://www.gnu.org/licenses/>

*/

#include <QtQuick>
#include <sailfishapp.h>

#include <QtDBus/QDBusArgument>

#include <TelepathyQt/Debug>
#include <TelepathyQt/Constants>
#include <TelepathyQt/Types>
#include <QObject>

#include "accounts-model.h"
#include "roster-model.h"
#include "pingyou-settings.h"
#include "filemodel.h"

int main(int argc, char *argv[])
{   
    QCoreApplication::setOrganizationName("PingYou");
    QCoreApplication::setApplicationName("PingYou");
    QCoreApplication::setApplicationVersion("0.0.1");

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    // Register types with dbus to prevent errors like "type Tp::UIntList is not registered with QtDBus"
    Tp::registerTypes();

    // create c++ items
    AccountsModel *accountsModel = new AccountsModel();
    RosterModel *rosterModel = new RosterModel();
    FileModel *fileModel = new FileModel();
    PingYouSettings * settings = new PingYouSettings();

    // register settings object with qml engine
    qmlRegisterType<PingYouSettings>("harbour.pingyou", 0, 1, "PingYouSettings");

    // expose the c++ objects to qml https://doc.qt.io/qt-4.8/qdeclarativemodels.html
    view->rootContext()->setContextProperty("accountsModel", accountsModel);
    view->rootContext()->setContextProperty("rosterModel",rosterModel);
    view->rootContext()->setContextProperty("fileModel",fileModel);
    view->rootContext()->setContextProperty("settings",settings);

    QObject::connect(accountsModel, SIGNAL(newAccountPtr(Tp::AccountPtr)),
            rosterModel, SLOT(setAccount(Tp::AccountPtr)));

    view->setSource(SailfishApp::pathTo("qml/harbour-pingyou.qml"));
    view->showFullScreen();
    app->exec();
}

