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


#include "accounts-model.h"

#include <TelepathyQt/Types>
#include <TelepathyQt/Account>
#include <TelepathyQt/AccountFactory>
#include <TelepathyQt/AccountManager>
#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/SharedPtr>
#include <TelepathyQt/DBusProxy>
#include <QVariant>
#include <QLatin1String>
#include <TelepathyQt/Connection>
#include <QDebug>

//AccountElement::AccountElement(Tp::AccountPtr acc, QObject *parent) : mAcc(acc) {
//    //Q_UNUSED(acc);
//    Q_UNUSED(parent);
//}


// ==========================================================


AccountsModel::AccountsModel(QObject *parent) {
    Q_UNUSED(parent);

    mAM = Tp::AccountManager::create(
                Tp::AccountFactory::create(QDBusConnection::sessionBus(),
                Tp::Account::FeatureCore)
                );

    connect(mAM->becomeReady(),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onAMReady(Tp::PendingOperation*))
            );

    connect(mAM.data(),
            SIGNAL(newAccount(const Tp::AccountPtr &)),
            SLOT(addAccountElement(const Tp::AccountPtr &))
            );

}

int AccountsModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent); // Q_UNUSED disables unused variable warnings
    return myList.size();
}


QVariant AccountsModel::data(const QModelIndex &index, int role) const {

    qDebug() << "role is: " << role;

    if (role == AccountRole) {
        Tp::Account * account = myList.at(index.row()).data();
        // need to use QVariant::fromValue to create a QVariant from a pointer https://stackoverflow.com/questions/34240236/qvariantqvariantvoid-is-private
        return QVariant::fromValue(account);
    }
    return QVariant();


//    AccountElement * fobj = myList.at(index.row());

//    if (role == ColumnValid) {
////        //return QVariant::fromValue(fobj->mAcc->isValid() ? QLatin1String("true") : QLatin1String("false"));

////        bool valid = fobj->mAcc->isValid();
////        //return QVariant::fromValue( valid ? QLatin1String("true") : QLatin1String("false"));

////        if (valid) {
////            return QVariant("true");
////            //return QVariant::fromValue(QLatin1String("true"));
////        }
////        else {
////            //return QVariant::fromValue(QLatin1String("false"));
////            return QVariant("false");
////        }

//        return QVariant::fromValue(fobj->mAcc->isValid() ? QVariant("true") : QVariant("false"));
//    }
//    else if (role == ColumnEnabled) {
//        //return QVariant::fromValue(fobj->mAcc->isEnabled() ? "true" : "false");
//        return QVariant::fromValue(fobj->mAcc->isEnabled() ? QVariant("true") : QVariant("false"));
//    }
//    else if (role == ColumnConnectionManager)
//        return QVariant::fromValue(fobj->mAcc->cmName());
//    else if (role == ColumnProtocol)
//        return QVariant::fromValue(fobj->mAcc->protocolName());
//    else if (role == ColumnDisplayName)
//        return QVariant::fromValue(fobj->mAcc->displayName());
//    else if (role == ColumnNickname)
//        return QVariant::fromValue(fobj->mAcc->nickname());
//    else if (role == ColumnConnectsAutomatically) {
//        //return QVariant::fromValue(fobj->mAcc->connectsAutomatically() ? "true" : "false");
//        return QVariant::fromValue(fobj->mAcc->connectsAutomatically() ? QVariant("true") : QVariant("false"));
//    }
//    else if (role == ColumnChangingPresence) {
//        //return QVariant::fromValue(fobj->mAcc->isChangingPresence() ? "true" : "false");
//        return QVariant::fromValue(fobj->mAcc->isChangingPresence() ? QVariant("true") : QVariant("false"));
//    }
//    else if (role == ColumnAutomaticPresence)
//        return QVariant::fromValue(fobj->mAcc->automaticPresence().status());
//    else if (role == ColumnCurrentPresence)
//        return QVariant::fromValue(fobj->mAcc->currentPresence().status());
//    else if (role == ColumnRequestedPresence)
//        return QVariant::fromValue(fobj->mAcc->requestedPresence().status());
//    else if (role == ColumnConnectionStatus) {
//        //return QVariant::fromValue(fobj->mAcc->connectionStatus());
//        return QVariant::fromValue(QVariant(fobj->mAcc->connectionStatus()));
//    }
//    else if (role == ColumnConnection)
//        return QVariant::fromValue(fobj->mAcc->connection().isNull() ? "" : fobj->mAcc->connection()->objectPath());

//    return QVariant();
}


QHash<int, QByteArray> AccountsModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[AccountRole] = "account";

//    roles[ColumnValid] = "valid";
//    roles[ColumnEnabled] = "enabled";
//    roles[ColumnConnectionManager] = "connection_manager";
//    roles[ColumnProtocol] = "protocol";
//    roles[ColumnDisplayName] = "display_name";
//    roles[ColumnNickname] = "nickname";
//    roles[ColumnConnectsAutomatically] = "connects_automatically";
//    roles[ColumnChangingPresence] = "changing_presence";
//    roles[ColumnAutomaticPresence] = "automatic_presence";
//    roles[ColumnCurrentPresence] = "current_presence";
//    roles[ColumnRequestedPresence] = "requested_presence";
//    roles[ColumnConnectionStatus] = "connection_status";
//    roles[ColumnConnection] = "connection";

    return roles;
}

void AccountsModel::onAMReady(Tp::PendingOperation *op) {

    Q_UNUSED(op);

    qDebug() << "onAMReady called";

    // add each account item to the model
    foreach (const Tp::AccountPtr &acc, mAM->allAccounts()) {
        // only add jabber accounts
        if ( QString::compare(acc->protocolName(), "jabber", Qt::CaseSensitive) == 0 ) {
            //qDebug() << "Adding account " << acc->displayName() << " from onAMReady";
            addAccountElement(acc);
        } else {
            qDebug() << "Skipped account with display name " << acc->displayName() << " because it is of type " << acc->protocolName();
        }
    }
}

//void AccountsModel::onNewAccount(const Tp::AccountPtr &acc) {
//    // TODO: add new item to model
//}

void AccountsModel::addAccountElement(const Tp::AccountPtr &acc) {
    qDebug() << "addAccountElement called, adding account with display name " << acc->displayName();

    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    //myList.append(new AccountElement(acc));
    myList.append(acc);
    endInsertRows();
}
