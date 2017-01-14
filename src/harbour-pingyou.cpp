/*

PingYou

Copyright (C) 2015 Sam Hobbs

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
//#include <QDebug>
//#include <QtCore>
//#include "protocols.h"
#include <QObject>

#include "accounts-model.h"
#include "roster-model.h"

int main(int argc, char *argv[])
{   
    QCoreApplication::setOrganizationName("PingYou");
    QCoreApplication::setApplicationName("PingYou");
    QCoreApplication::setApplicationVersion("0.0.1");

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    // Register types with dbus to prevent errors like "type Tp::UIntList is not registered with QtDBus"
    Tp::registerTypes();

    // create AccountsModel item that will be exposed to QML
    AccountsModel *accountsModel = new AccountsModel();
    RosterModel *rosterModel = new RosterModel();

    // register types with qml engine
    //qmlRegisterType<Tp::Account>();
    //qmlRegisterType<Tp::Account>("harbour.pingyou",1,0,"Account"); // gives error "no matching function for call to Tp::Account::Account()" because account object has no default constructor (required for qml to initialise object)
    //qmlRegisterExtendedType<Tp::Account>();
    //qRegisterMetaType<Tp::Account>("Tp::Account");

    //qmlRegisterType<Tp::Connection>();
    //qmlRegisterType<Tp::Presence>(); // gives error "staticMetaObject is not a member of Tp::Presence" because Q_OBJECT macro is not defined in the class
    //qmlRegisterType<Tp::ConnectionStatus>(); // gives error "staticMetaObject is not a member of Tp::ConnectionStatus" because Q_OBJECT macro is not defined in the class

    // expose the accountsModel data model to qml https://doc.qt.io/qt-4.8/qdeclarativemodels.html
    view->rootContext()->setContextProperty("accountsModel", accountsModel);
    view->rootContext()->setContextProperty("rosterModel",rosterModel);

    QObject::connect(accountsModel, SIGNAL(newAccountPtr(Tp::AccountPtr)),
            rosterModel, SLOT(setAccount(Tp::AccountPtr)));

    view->setSource(SailfishApp::pathTo("qml/harbour-pingyou.qml"));
    view->showFullScreen();
    app->exec();
}

