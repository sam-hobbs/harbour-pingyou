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
#include <TelepathyQt/PendingAccount>

//#include "account-properties-dialog-data.h"

AccountElement::AccountElement(Tp::AccountPtr acc, QObject *parent) : QObject(parent), mAcc(acc), mAvatarFile(new QTemporaryFile(this)) {

    Q_ASSERT(!mAcc.isNull());

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

    connect(mAcc.data(), SIGNAL(normalizedNameChanged(QString)),
            this, SIGNAL(normalisedNameChanged(QString)));

    connect(mAcc.data(), SIGNAL(connectionStatusChanged(Tp::ConnectionStatus)),
            this, SIGNAL(connectionStatusReasonChanged()));

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
    //qDebug() << "nickname is: " << mAcc->nickname();
    return mAcc->nickname();
    //return QString::fromUtf8(mAcc->nickname());
}

bool AccountElement::connectsAutomatically() const {
    return mAcc->connectsAutomatically();
}

bool AccountElement::changingPresence() const {
    return mAcc->isChangingPresence();
}

// Tp::Presence is a struct (uint type, qstring status, qstring status_message)
// https://telepathy.freedesktop.org/spec/Connection_Interface_Simple_Presence.html#Struct:Simple_Presence
QString AccountElement::automaticPresence() const {
    return mAcc->automaticPresence().status();
}

QString AccountElement::currentPresence() const {
    //qDebug() << mAcc->currentPresence().status();
    return mAcc->currentPresence().status();
}

QString AccountElement::requestedPresence() const {
    return mAcc->requestedPresence().status();
}

Tp::ConnectionStatus AccountElement::connectionStatus() const {
    return mAcc->connectionStatus();
}

Tp::ConnectionStatusReason AccountElement::connectionStatusReason() const {

    if ( !mAcc->connectionError().isEmpty() )
        qDebug() << "Connection error for account" << mAcc->displayName() << " is: " << mAcc->connectionError() << ", connection error details are: " << mAcc->connectionErrorDetails().allDetails();

    return mAcc->connectionStatusReason();
}

QString AccountElement::connectionPath() const {
    return mAcc->connection().isNull() ? QString("") : mAcc->connection()->objectPath();
}

bool AccountElement::online() const {
    return mAcc->isOnline();
}

QString AccountElement::normalisedName() const {
    return mAcc->normalizedName();
}

QString AccountElement::avatarPath() const {
    // avatar() returns Tp::Avatar instead of Tp::AvatarData like the contact interface. Avatar data has been written to a temporary file using Tp::Avatar.avatarData, which returns a QByteArray

    if (QFile(mAvatarFile->fileName()).size() == 0) {
        //qDebug() << "avatarPath called, returning empty string because the file is empty";
        return "";
    } else {
        //qDebug() << "avatarPath called, path is: " << mAvatarFile->fileName();
        return mAvatarFile->fileName();
    }
}

void AccountElement::avatarChanged(Tp::Avatar avatar) {
    //qDebug() << "avatar changed, writing to temporary file";

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

//    qDebug() << "listing protocol parameters";
//    //mAcc->protocolInfo().parameters(); // ProtocolParameterList
//    foreach (Tp::ProtocolParameter parameter, mAcc->protocolInfo().parameters()) {
//        qDebug() << "parameter name: " << parameter.name() << ", default value: " << parameter.defaultValue() << ", required: " << parameter.isRequired() << ", required for registration: " << parameter.isRequiredForRegistration();
//    }
//    qDebug();

//    qDebug() << "listing account profile parameters";
//    foreach (Tp::Profile::Parameter parameter, mAcc->profile()->parameters()) { //ParameterList
//        qDebug() << "name: " << parameter.name() << ", value : " << parameter.value() << ", mandatory is: " << parameter.isMandatory();
//    }

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


void AccountElement::setConnectsAutomatically(bool setting) {
    // check that we are setting something different to the current value
    if (setting != connectsAutomatically())
        mAcc->setConnectsAutomatically(setting);

    // TODO: connect pending operation to a generic error handling slot
}

void AccountElement::setEnabled(bool setting) {
    // check that we are setting something different to the current value
    if (setting != enabled())
        mAcc->setEnabled(setting);
}

void AccountElement::reconnect() {
    qDebug() << "Reconnect called";
    AccountsModel * accountsModel = dynamic_cast<AccountsModel *>(this->parent());
    connect(mAcc->reconnect(),SIGNAL(finished(Tp::PendingOperation*)),
            accountsModel,SLOT(genericErrorHandler(Tp::PendingOperation*))
            );
}


void AccountElement::setAutomaticPresence(int statusNum, QString statusString) {
    qDebug() << "setAutomaticPresence called";
    AccountsModel * accountsModel = dynamic_cast<AccountsModel *>(this->parent());

    Tp::SimplePresence presence;
    presence.type=statusNum;
    presence.status=statusString;

    connect(mAcc->setAutomaticPresence(Tp::Presence(presence)),SIGNAL(finished(Tp::PendingOperation*)),
            accountsModel,SLOT(genericErrorHandler(Tp::PendingOperation*))
            );
}

void AccountElement::setRequestedPresence(int statusNum, QString statusString) {
    qDebug() << "setRequestedPresence called";
    AccountsModel * accountsModel = dynamic_cast<AccountsModel *>(this->parent());

    Tp::SimplePresence presence;
    presence.type=statusNum;
    presence.status=statusString;

    connect(mAcc->setRequestedPresence(Tp::Presence(presence)),SIGNAL(finished(Tp::PendingOperation*)),
            accountsModel,SLOT(genericErrorHandler(Tp::PendingOperation*))
            );
}

QString AccountElement::connectionError() const {
    return mAcc->connectionError();
}

QVariant AccountElement::connectionErrorDetails() const {
    QVariantMap map = mAcc->connectionErrorDetails().allDetails();
    return QVariant::fromValue(map);
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

    //qDebug() << "onAMReady called";

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
    //qDebug() << "addAccountElement called, adding account with display name " << acc->displayName();

    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    AccountElement * newAccount = new AccountElement(acc,this);

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

    connect(newAccount,SIGNAL(onlineChanged(bool)),
            SIGNAL(numOnlineAccountsChanged())
            );

    myList.append(newAccount);

    endInsertRows();

    emit numValidAccountsChanged();
    emit numOnlineAccountsChanged();
}


int AccountsModel::numValidAccounts() const {

    int numValid = 0;

    // loop through the list of AccountElement objects and if the element is valid and enabled, add it to the count
    foreach (AccountElement * account, myList) {
        if (account->valid() && account->enabled()) {
            numValid++;
        }
    }

    //qDebug() << "number of valid accounts is: " << numValid;
    return numValid;
}

int AccountsModel::numOnlineAccounts() const {
    int numOnline = 0;

    // loop through the list of AccountElement objects and count number of online accounts
    foreach (AccountElement * account, myList) {
        if (account->online()) {
            numOnline++;
        }
    }
    return numOnline;
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
            emit numOnlineAccountsChanged();
        } else {
            qDebug() << "Item " << i << "does not match the account to be removed";
        }
    }

}


///**
// * Update this account parameters.
// *
// * On success, the PendingOperation returned by this method will produce a
// * list of strings, which are the names of parameters whose changes will not
// * take effect until the account is disconnected and reconnected (for instance
// * by calling reconnect()).
// *
// * \param set Parameters to set.
// * \param unset Parameters to unset.
// * \return A PendingStringList which will emit PendingStringList::finished
// *         when the request has been made
// * \sa parametersChanged(), parameters(), reconnect()
// */
//PendingStringList *Account::updateParameters(const QVariantMap &set,
//        const QStringList &unset)
//{
//    return new PendingStringList(
//            baseInterface()->UpdateParameters(set, unset),
//            AccountPtr(this));
//}

/**
 * Create an account with the given parameters.
 *
 * The optional \a properties argument can be used to set any property listed in
 * supportedAccountProperties() at the time the account is created.
 *
 * \param connectionManager The name of the connection manager to create the account
 *                          for.
 * \param protocol The name of the protocol to create the account for.
 * \param displayName The account display name.
 * \param parameters The account parameters.
 * \param properties An optional map from fully qualified D-Bus property
 *                   names such as "org.freedesktop.Telepathy.Account.Enabled"
 *                   to their values.
 * \return A PendingAccount object which will emit PendingAccount::finished
 *         when the account has been created of failed its creation process.
 * \sa supportedAccountProperties()
 */


void AccountsModel::prepareParameterQVariantMap(QVariantMap parameters) {

    // iterate through the qvariantmap and remove empty strings
    QVariantMap::iterator iter = parameters.begin();
    while( iter !=parameters.end() ) {
        //qDebug() << "Current item is " << iter.key() << iter.value();
        if (iter.value() == QVariant(QString(""))) {
            //qDebug() << "removing empty string";
            iter = parameters.erase(iter);
        } else {
            ++iter;
        }
    }

    // change types
    if (parameters.contains("keepalive-interval")) parameters["keepalive-interval"]= QVariant(parameters.value("keepalive-interval").toUInt());
    if (parameters.contains("https-proxy-port")) parameters["https-proxy-port"]= QVariant(parameters.value("https-proxy-port").toUInt());
    if (parameters.contains("priority")) parameters["priority"]= QVariant(parameters.value("priority").toInt());
    if (parameters.contains("fallback-cocks5-proxies")) parameters["fallback-socks5-proxies"]= QVariant(parameters.value("fallback-socks5-proxies").toStringList());
    if (parameters.contains("extra-certificate-identities")) parameters["extra-certificate-identities"]= QVariant(parameters.value("extra-certificate-identities").toStringList());
    if (parameters.contains("fallback-servers")) parameters["fallback-servers"]= QVariant(parameters.value("fallback-servers").toStringList());
    if (parameters.contains("fallback-stun-port")) parameters["fallback-stun-port"]= QVariant(parameters.value("fallback-stun-port").toUInt());
    if (parameters.contains("port")) parameters["port"]= QVariant(parameters.value("port").toUInt());
    if (parameters.contains("stun-port")) parameters["stun-port"]= QVariant(parameters.value("stun-port").toUInt());

    // TODO: change dialog so account is required before dialog is accepted
    Q_ASSERT(parameters.contains("account") && !parameters.value("account").toString().isEmpty());

    QString accountName = parameters.value("account").toString();
    bool exists = false;

    // if the parameters are for an account that already exists, amend the parameters
    foreach (AccountElement * account, myList) {
        //qDebug() << "Checking account " << account->displayName();
        if (account->displayName() == accountName) {
            qDebug() << "Account " << account->displayName() << " already exists, changing account parameters";
            exists = true;
            changeAccountParameters(account->getAccountPtr(),parameters);
            break;
        }
    }

    // if the account does not exist, create a new account
    if (!exists) {
        qDebug() << "Account " << parameters.value("account").toString() << " does not exist, creating new account";
        createAccount(parameters);
    }
}



void AccountsModel::createAccount(QVariantMap parameters) {

    // create a qvariantmap of properties to use in account creation
    QVariantMap properties;
    properties.insert("org.freedesktop.Telepathy.Account.Enabled",QVariant(true));
    properties.insert("org.freedesktop.Telepathy.Account.ConnectAutomatically",QVariant(true));

    // create an account with display name equal to the account name
    Tp::PendingAccount * pendingAccount = mAM->createAccount("gabble",
                       "jabber",
                       parameters.value("account").toString(),
                       parameters);


    connect(pendingAccount, SIGNAL(finished(Tp::PendingOperation*)),
            this, SLOT(onAccountCreationFinished(Tp::PendingOperation*))
            );
}


void AccountsModel::changeAccountParameters(Tp::AccountPtr account,QVariantMap newParams) {

//    qDebug() << "changeAccountParameters called";
    QVariantMap currentParams = account->parameters();

//    qDebug() << "printing current parameters";
//    qDebug() << currentParams;
//    qDebug() << "printing new parameters";
//    qDebug() << newParams;

    QVariantMap setParams; // parameters to be set
    QStringList unsetParams; // parameters to be unset

    // loop through the current parameters to find ones that have been removed
    QVariantMap::iterator iter = currentParams.begin();
    while( iter !=currentParams.end() ) {
        // if parameter is in currentParams but not in newParams, it has been unset
        if (!newParams.contains(iter.key())) {
            //qDebug() << iter.key() << " will be unset";
            unsetParams.append(iter.key());
        }
        ++iter;
    }

    // loop through the new parameters to find ones that have changed
    iter = newParams.begin();
    while( iter !=newParams.end() ) {
        // if parameter is not in currentParams, it is new
        if (!currentParams.contains(iter.key())) {
            //qDebug() << iter.key() << " is new and will be set";
            setParams.insert(iter.key(),iter.value());
        } else {
            // if parameter is in currentParams and newParams and the values are equal, value has not changed (do nothing)
            // if parameter is in currentParams and newParams and the values are different, update the parameter
            if(iter.value() == currentParams.value(iter.key())) {
                //qDebug() << iter.key() << " value has not changed";
            } else {
                //qDebug() << iter.key() << " value has changed and will be updated";
                setParams.insert(iter.key(),iter.value());
            }
        }
        ++iter;
    }

    // TODO: connect pendingstringlist to error handling slot
    account->updateParameters(setParams, unsetParams);

}

void AccountsModel::onAccountCreationFinished(Tp::PendingOperation * op) {
    if ( op->isError() ) {
        qWarning() << "Error creating account: " << op->errorName() << ", " << op->errorMessage();
    } else {
        qDebug() << "Successfully created account";
    }
}

void AccountsModel::genericErrorHandler(Tp::PendingOperation *op) {
    qDebug() << "Generic error handler called";
    if ( op->isError() ) {
        qWarning() << "Error: " << op->errorName() << ", " << op->errorMessage();
    } else {
        qDebug() << "Success";
    }
}
