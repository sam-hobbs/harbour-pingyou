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
#include <TelepathyQt/ConnectionFactory>
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
#include <TelepathyQt/AccountSet>
#include <TelepathyQt/Profile>
#include <TelepathyQt/ProtocolInfo>

AccountElement::AccountElement(Tp::AccountPtr acc, QObject *parent) : QObject(parent), mAcc(acc), mAvatarFile(new QTemporaryFile(this)) {

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

    avatarChanged(mAcc->avatar());
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
    // avatar() returns Tp::Avatar instead of Tp::AvatarData like the contact interface. Avatar data has been written to a temporary file using Tp::Avatar.avatarData, which returns a QByteArray
    qDebug() << "avatarPath called, path is: " << mAvatarFile->fileName();
    return mAvatarFile->fileName();
}

void AccountElement::avatarChanged(Tp::Avatar avatar) {
    qDebug() << "avatar changed, writing to temporary file";

    // QML Image element doesn't reload the image if the source filename doesn't change. Therefore, create a new temporary file (with a different path), swap it out and delete the old one
    QTemporaryFile * oldAvatar = mAvatarFile;
    mAvatarFile = new QTemporaryFile(this);
    delete oldAvatar;

    // write the new data to the temporary file
    mAvatarFile->open();
    mAvatarFile->write(avatar.avatarData);
    mAvatarFile->close();
    emit avatarPathChanged(mAvatarFile->fileName());
}

Tp::AccountPtr AccountElement::getAccountPtr() {
    return mAcc;
}

// method invokable from QML for emitting signal that sets this account active in roster etc.
void AccountElement::setActiveAccount() {
    emit setThisAccountActive(getAccountPtr());
}


QVariant AccountElement::parameterList() const {
    qDebug() << "listing account parameters";
    qDebug() << mAcc->parameters();
    qDebug();

    qDebug() << "listing protocol parameters";
    //mAcc->protocolInfo().parameters(); // ProtocolParameterList
    foreach (Tp::ProtocolParameter parameter, mAcc->protocolInfo().parameters()) {
        qDebug() << "parameter name: " << parameter.name() << ", default value: " << parameter.defaultValue() << ", required: " << parameter.isRequired() << ", required for registration: " << parameter.isRequiredForRegistration();
    }
    qDebug();

    qDebug() << "listing account profile parameters";
    foreach (Tp::Profile::Parameter parameter, mAcc->profile()->parameters()) { //ParameterList
        qDebug() << "name: " << parameter.name() << ", value : " << parameter.value() << ", mandatory is: " << parameter.isMandatory();
    }

    return QVariant::fromValue(mAcc->parameters());
}


void AccountElement::remove() {

    // "mAcc->remove()" will delete the account object and the account pointer will become invalid. need to check the result of the deletion. If successful, trigger removal of this AccountElement from the model
    connect(mAcc->remove(), SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(accountRemovalResultHandler(Tp::PendingOperation*))
            );
}

void AccountElement::accountRemovalResultHandler(Tp::PendingOperation * op) {
    if ( op->isError() ) {
        qWarning() << "Error deleting account object: " << op->errorName() << ", " << op->errorMessage();
    } else {
        qDebug() << "Asking account model to delete this object";
        emit deleteMe(this);
    }
}

void AccountElement::toggleEnabled() {
    mAcc->setEnabled(!mAcc->isEnabled());
    // TODO: connect pending operation to a generic error handling slot
}

// ==========================================================


AccountsModel::AccountsModel(QObject *parent) {
    Q_UNUSED(parent);

//    mAM = Tp::AccountManager::create(
//                Tp::AccountFactory::create(QDBusConnection::sessionBus(),
//                Tp::Account::FeatureCore)
//                );

    mAM = Tp::AccountManager::create(
                Tp::AccountFactory::create(QDBusConnection::sessionBus(),Tp::Account::FeatureCore | Tp::Account::FeatureAvatar | Tp::Account::FeatureProfile), // unused options: Tp::Account::FeatureAvatar, Tp::Account::FeatureCapabilities, Tp::Account::FeatureProfile used to list parameters, Tp::Account::FeatureProtocolInfo
                Tp::ConnectionFactory::create(QDBusConnection::sessionBus(), Tp::Connection::FeatureConnected | Tp::Connection::FeatureRoster | Tp::Connection::FeatureRosterGroups), // unused options: Tp::Connection::FeatureAccountBalance, Tp::Connection::FeatureCore, Tp::Connection::FeatureSelfContact, Tp::Connection::FeatureSimplePresence
                Tp::ChannelFactory::create(QDBusConnection::sessionBus()),
                Tp::ContactFactory::create(Tp::Contact::FeatureAlias | Tp::Contact::FeatureSimplePresence | Tp::Contact::FeatureAvatarData | Tp::Contact::FeatureAvatarToken) // unused options: Tp::Contact::FeatureAddresses, Tp::Contact::FeatureAvatarData, Tp::Contact::FeatureAvatarToken, Tp::Contact::FeatureCapabilities, Tp::Contact::FeatureClientTypes, Tp::Contact::FeatureInfo, Tp::Contact::FeatureLocation, Tp::Contact::FeatureRosterGroups
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

    if (role == AccountRole) {
        return QVariant::fromValue(myList.at(index.row()));
    }

    return QVariant();
}


QHash<int, QByteArray> AccountsModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[AccountRole] = "account";

    return roles;
}

void AccountsModel::onAMReady(Tp::PendingOperation *op) {

    Q_UNUSED(op);

    qDebug() << "onAMReady called";

    // query for jabber accounts only. mAM->allAccounts() returns telephony accounts too, which we don't want
    foreach(const Tp::AccountPtr &acc, mAM->accountsByProtocol(QString("jabber"))->accounts() ) {
        qDebug() << "Adding account " << acc->displayName();
        addAccountElement(acc);
    }

    // if number of valid accounts is 1 or more, emit signal with account pointer so that the rosterModel can update its data
    if (numValidAccounts() > 0) {
        foreach (AccountElement * account, myList) {
            if (account->valid() && account->enabled()) {
                //qDebug() << "valid and enabled account found, emitting signal";
                emit newAccountPtr(account->getAccountPtr());
                break;
            }
        }
    }
}

void AccountsModel::addAccountElement(const Tp::AccountPtr &acc) {
    qDebug() << "addAccountElement called, adding account with display name " << acc->displayName();

    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    AccountElement * newAccount = new AccountElement(acc);

    // connect signals and slots to allow the element to delete itself from the model
    connect(newAccount, SIGNAL(deleteMe(AccountElement*)),
            SLOT(removeAccount(AccountElement*))
            );

    // when the new AccountElement requests that it become the active account in the rosterModel, pass on the signal
    connect(newAccount,SIGNAL(setThisAccountActive(Tp::AccountPtr)),
            SIGNAL(newAccountPtr(Tp::AccountPtr)));

    // update numValidAccounts when valid or enabled properties change
    connect(newAccount,SIGNAL(enabledChanged(bool)),
            SIGNAL(numValidAccountsChanged())
            );
    connect(newAccount,SIGNAL(validChanged(bool)),
            SIGNAL(numValidAccountsChanged())
            );

    myList.append(newAccount);

    endInsertRows();

    emit numValidAccountsChanged();
}


bool AccountsModel::numValidAccounts() const {

    int numValid = 0;

    // loop through the list of AccountElement objects and if the element is valid and enabled, add it to the count
    foreach (AccountElement * account, myList) {
        if (account->valid() && account->enabled()) {
            numValid++;
        }
    }

    qDebug() << "number of valid accounts is: " << numValid;
    return numValid;
}

void AccountsModel::removeAccount(AccountElement * account) {
    qDebug() << "removeAccount called";

    // find the account to be deleted in the list, and remove + delete it
    for ( int i=0; i< myList.count(); ++i ) {
        if (myList[i] == account) {
            qDebug() << "Item " << i << " matches the account to be removed. Removing from list and deleting";
            beginRemoveRows(QModelIndex(),i,i);
            AccountElement * item = myList.takeAt(i);
            endRemoveRows();
            delete item;
            emit numValidAccountsChanged();
        } else {
            qDebug() << "Item " << i << "does not match the account to be removed";
        }
    }

}
