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
#include <TelepathyQt/AvatarData>
#include <TelepathyQt/AvatarSpec>

AccountElement::AccountElement(Tp::AccountPtr acc, QObject *parent) : QObject(parent), mAcc(acc) {

    // bind signals
    connect(mAcc.data(), SIGNAL(validityChanged(bool)),
            this, SIGNAL(validChanged(bool)));

    connect(mAcc.data(), SIGNAL(stateChanged(bool)),
            this, SIGNAL(enabledChanged(bool)));

    // cmName is not notifiable
    // protocolName is not notifiable

    connect(mAcc.data(), SIGNAL(displayNameChanged(QString)),
            this, SIGNAL(displayNameChanged(QString)));

    connect(mAcc.data(), SIGNAL(nicknameChanged(QString)),
            this, SIGNAL(nicknameChanged(QString)));

    connect(mAcc.data(), SIGNAL(connectsAutomaticallyPropertyChanged(bool)),
            this, SIGNAL(connectsAutomaticallyChanged(bool)));

    connect(mAcc.data(), SIGNAL(changingPresence(bool)),
            this, SIGNAL(changingPresenceChanged(bool)));

    connect(mAcc.data(), SIGNAL(automaticPresenceChanged(Tp::Presence)),
            this, SIGNAL(automaticPresenceChanged(Tp::Presence)));

    connect(mAcc.data(), SIGNAL(currentPresenceChanged(Tp::Presence)),
            this, SIGNAL(currentPresenceChanged(Tp::Presence)));

    connect(mAcc.data(), SIGNAL(requestedPresenceChanged(Tp::Presence)),
            this, SIGNAL(requestedPresenceChanged(Tp::Presence)));

    connect(mAcc.data(), SIGNAL(connectionStatusChanged(Tp::ConnectionStatus)),
            this, SIGNAL(connectionStatusChanged(Tp::ConnectionStatus)));

    connect(mAcc.data(), SIGNAL(connectionChanged(Tp::ConnectionPtr)),
            this, SIGNAL(connectionPathChanged(Tp::ConnectionPtr)));

    connect(mAcc.data(), SIGNAL(onlinenessChanged(bool)),
            this, SIGNAL(onlineChanged(bool)));

    connect(mAcc.data(), SIGNAL(avatarChanged(Tp::Avatar)),
            this, SLOT(avatarChanged(Tp::Avatar)));


}


bool AccountElement::valid() const {
    return mAcc->isValid();
}

bool AccountElement::enabled() const {
    return mAcc->isEnabled();
}

QString AccountElement::cmName() const {
    return mAcc->cmName();
}

QString AccountElement::protocolName() const {
    return mAcc->protocolName();
}

QString AccountElement::displayName() const {
    return mAcc->displayName();
}

QString AccountElement::nickname() const {
    qDebug() << "nickname is: " << mAcc->nickname();
    return mAcc->nickname();
    //return QString::fromUtf8(mAcc->nickname());
}

bool AccountElement::connectsAutomatically() const {
    return mAcc->connectsAutomatically();
}

bool AccountElement::changingPresence() const {
    return mAcc->isChangingPresence();
}

QString AccountElement::automaticPresence() const {
    return mAcc->automaticPresence().status();
}

QString AccountElement::currentPresence() const {
    return mAcc->currentPresence().status();
}

QString AccountElement::requestedPresence() const {
    return mAcc->requestedPresence().status();
}

Tp::ConnectionStatus AccountElement::connectionStatus() const {
    return mAcc->connectionStatus();
}

QString AccountElement::connectionPath() const {
    return mAcc->connection().isNull() ? QString("") : mAcc->connection()->objectPath();
}

bool AccountElement::online() const {
    return mAcc->isOnline();
}

QString AccountElement::avatarPath() const {
    // TODO avatar() returns Tp::Avatar which is a bytearray but we want Tp::AvatarData
    //Tp::AvatarData avatar = mAcc->avatar();
    //qDebug() << "filename is " << avatar.fileName;
    //return avatar.fileName;
    //qDebug() << "mimetype is: " << mAcc->avatar.MIMEType; //Tp::Account::avatar does not have class type
    return QString("/usr/share/icons/hicolor/256x256/apps/harbour-pingyou.png"); // TODO: CHANGEME - just for testing
}

void AccountElement::avatarChanged(Tp::Avatar avatar) const {
    //Tp::AvatarData av = avatar;
    qDebug() << "avatar changed";
    //emit avatarPathChanged(av.fileName);
}

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

    //
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

    if (role == AccountRole) {
        return QVariant::fromValue(myList.at(index.row()));
    }

    return QVariant();
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

void AccountsModel::addAccountElement(const Tp::AccountPtr &acc) {
    qDebug() << "addAccountElement called, adding account with display name " << acc->displayName();

    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    myList.append(new AccountElement(acc));
    //myList.append(acc);
    endInsertRows();
}
