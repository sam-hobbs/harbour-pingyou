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

#include <TelepathyQt/Types>
#include <TelepathyQt/Account>
#include <TelepathyQt/Contact>
//#include <TelepathyQt/ContactCapabilities>
#include <TelepathyQt/ContactManager>
#include <TelepathyQt/PendingConnection>
#include <TelepathyQt/PendingContacts>
#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/Presence>
#include <QObject>
#include <QDebug>
#include <QEventLoop>


#include "roster-model.h"

RosterElement::RosterElement (Tp::ContactPtr contact, QObject *parent) : QObject(parent), mContact(contact) {
    onContactChanged();

    Q_ASSERT(!contact.isNull());

    // if the contact alias, presence, subscription state or block status change, update the object
    connect(contact.data(),
            SIGNAL(aliasChanged(QString)),
            SLOT(onContactChanged())
            );

    connect(contact.data(),
            SIGNAL(presenceChanged(Tp::Presence)),
            SLOT(onContactChanged())
            );

    connect(contact.data(),
            SIGNAL(subscriptionStateChanged(Tp::Contact::PresenceState)),
            SLOT(onContactChanged())
            );

    connect(contact.data(),
            SIGNAL(blockStatusChanged(bool)),
            SLOT(onContactChanged())
            );

    connect(contact.data(),
            SIGNAL(avatarTokenChanged(QString)),
            SLOT(onAvatarTokenChanged(QString))
            );

    connect(contact.data(),
            SIGNAL(avatarDataChanged(Tp::AvatarData)),
            SLOT(onAvatarDataChanged(Tp::AvatarData))
            );

    // connect signals
    connect(contact.data(),
            SIGNAL(publishStateChanged(Tp::Contact::PresenceState,QString)),
            SIGNAL(publishStateChanged())
            );

    connect(contact.data(),
            SIGNAL(subscriptionStateChanged(Tp::Contact::PresenceState)),
            SIGNAL(subscriptionStateChanged())
            );

    connect(contact.data(),
            SIGNAL(blockStatusChanged(bool)),
            SIGNAL(blockedChanged())
            );

    mContact->requestAvatarData();

}

RosterElement::~RosterElement() {
    qDebug() << "RosterElement destructor called";
}


void RosterElement::onContactChanged() {

    Q_ASSERT(!mContact.isNull());

    mStatus = mContact->presence().status();
    emit statusChanged();

}

QString RosterElement::contactID() const {

    Q_ASSERT(!mContact.isNull());

    return mContact->id();
}

QString RosterElement::status() const {
    return mStatus;
}

void RosterElement::authPubAction() {
    qDebug() << "Auth pub action triggered";

    Q_ASSERT(!mContact.isNull());

    if (mContact->publishState() != Tp::Contact::PresenceStateYes )
        mContact->authorizePresencePublication();
}

void RosterElement::denyPubAction() {
    qDebug() << "Deny pub action triggered";

    Q_ASSERT(!mContact.isNull());

    if (mContact->publishState() != Tp::Contact::PresenceStateNo)
        mContact->removePresencePublication();
}

void RosterElement::removePubAction() {
    qDebug() << "Remove pub action triggered";

    Q_ASSERT(!mContact.isNull());

    if(mContact->subscriptionState() != Tp::Contact::PresenceStateNo) {
        mContact->removePresencePublication();
        mContact->removePresenceSubscription();
    }
}

void RosterElement::toggleBlockAction() {
    qDebug() << "toggle block action triggered";

    Q_ASSERT(!mContact.isNull());

    if(mContact->isBlocked()) {
        qDebug() << "Unblocking contact";
        mContact->unblock();
    } else {
        qDebug() << "Blocking contact";
        mContact->block();
    }
}

void RosterElement::requestSubAction() {
    qDebug() << "request sub action triggered";

    Q_ASSERT(!mContact.isNull());

    if(mContact->subscriptionState() == Tp::Contact::PresenceStateNo) {
        mContact->requestPresenceSubscription();
    }
}


void RosterElement::rescindSubRequestAction() {
    qDebug() << "rescind sub request action triggered";

    Q_ASSERT(!mContact.isNull());

    if(mContact->subscriptionState() == Tp::Contact::PresenceStateAsk) {
        mContact->removePresenceSubscription();
    }
}






QString RosterElement::publishState() const {

    Q_ASSERT(!mContact.isNull());

    if (mContact->publishState() == Tp::Contact::PresenceStateYes) {
        return QString("yes");
    } else if (mContact->publishState() == Tp::Contact::PresenceStateNo) {
        return QString("no");
    } else if (mContact->publishState() == Tp::Contact::PresenceStateAsk) {
        return QString("ask");
    }

    return QString("");
}

QString RosterElement::subscriptionState() const {

    Q_ASSERT(!mContact.isNull());

    if (mContact->subscriptionState() == Tp::Contact::PresenceStateYes) {
        return QString("yes");
    } else if (mContact->subscriptionState() == Tp::Contact::PresenceStateNo) {
        return QString("no");
    } else if (mContact->subscriptionState() == Tp::Contact::PresenceStateAsk) {
        return QString("ask");
    }

    return QString("");
}

bool RosterElement::blocked() const {

    Q_ASSERT(!mContact.isNull());

    return mContact->isBlocked();
}


bool RosterElement::canAuthorisePublication() const {

    Q_ASSERT(!mContact.isNull());

    return mContact->manager()->canAuthorizePresencePublication();
}

bool RosterElement::canRemovePublication() const {

    Q_ASSERT(!mContact.isNull());

    return mContact->manager()->canRemovePresencePublication();
}

bool RosterElement::canBlockContacts() const {

    Q_ASSERT(!mContact.isNull());

    return mContact->manager()->canBlockContacts();
}

bool RosterElement::canRequestPresenceSubscription() const {

    Q_ASSERT(!mContact.isNull());

    return mContact->manager()->canRequestPresenceSubscription();
}

bool RosterElement::canRescindPresenceSubscriptionRequest() const {

    Q_ASSERT(!mContact.isNull());

    return mContact->manager()->canRescindPresenceSubscriptionRequest();
}

QString RosterElement::avatarPath() const {

    Q_ASSERT(!mContact.isNull());

    if (mContact->actualFeatures().contains(Tp::Contact::FeatureAvatarData)) {

        if (mContact->actualFeatures().contains(Tp::Contact::FeatureAvatarToken)) {
//            if ( mContact->isAvatarTokenKnown() ) {
//                qDebug() << mContact->id() << ": avatar and avatar token features are supported, token is " << mContact->avatarToken() << ", path is " << mContact->avatarData().fileName;
//            } else {
//                qDebug() << mContact->id() << ": avatar and avatar token features are supported, path is " << mContact->avatarData().fileName;
//            }
        } else {
            qDebug() << mContact->id() << ": avatar feature is supported, path is " << mContact->avatarData().fileName;
        }
        return mContact->avatarData().fileName;
    } else {
        qDebug() << mContact->id() << ": avatar is not available";
        return QString("");
    }
}

void RosterElement::onAvatarTokenChanged(QString avatarToken) {

    Q_ASSERT(!mContact.isNull());

    //qDebug() << mContact->id() << ": onAvatarTokenChanged called";
    if (mAvatarToken != avatarToken) {
        //qDebug() << "avatar token changed, emitting avatarPathChanged";
        mAvatarToken = avatarToken;
        emit avatarPathChanged(avatarPath());
    } else {
        //qDebug() << "avatar token has not changed, doing nothing";
    }
}

void RosterElement::onAvatarDataChanged(Tp::AvatarData data) {

    Q_ASSERT(!mContact.isNull());

    //qDebug() << mContact->id() << ": onAvatarDataChanged called";
    if (mContact->actualFeatures().contains(Tp::Contact::FeatureAvatarToken)) {
        //qDebug() << "not changing avatar path because tokens are supported";
    } else {
        emit avatarPathChanged(data.fileName);
    }
}

// =============================================================

RosterModel::RosterModel(QObject *parent) {
    Q_UNUSED(parent);
}

RosterModel::~RosterModel() {

}

void RosterModel::setConnection(const Tp::ConnectionPtr &conn) {

    //qDebug() << "setConnection called";

    //Q_ASSERT(!mConn.isNull());

    // if connection already exists, clear the list of roster items
    if (mConn) {
        unsetConnection();
    }

    // replace the old connection with the new one (shared ptr means it should be destroyed when it is no longer in use)
    mConn = conn;

    connect(conn->contactManager().data(),
            SIGNAL(presencePublicationRequested(const Tp::Contacts &)),
            SLOT(onPresencePublicationRequested(const Tp::Contacts &)));

    connect(conn->contactManager().data(),
            SIGNAL(allKnownContactsChanged(Tp::Contacts,Tp::Contacts,Tp::Channel::GroupMemberChangeDetails)),
            SLOT(onKnownContactsChanged(Tp::Contacts,Tp::Contacts,Tp::Channel::GroupMemberChangeDetails))
            );

    connect(conn->contactManager().data(),
            SIGNAL(stateChanged(Tp::ContactListState)),
            SLOT(onContactManagerStateChanged(Tp::ContactListState)));

    connect(conn.data(), SIGNAL(statusChanged(Tp::ConnectionStatus)),
            this, SIGNAL(connectedChanged()));

    onContactManagerStateChanged(conn->contactManager()->state());

}


void RosterModel::unsetConnection() {

    qDebug() << "unsetConnection called";

    // remove existing RosterElements from the list and then delete them
    while (mList.count() > 0) {
        RosterElement * item = mList.at(0); // get pointer to current item for deletion later
        beginRemoveRows(QModelIndex(), 0, 0);
        mList.takeAt(0);
        endRemoveRows();
        delete item;
    }

    //Q_ASSERT(!mConn.isNull());
    mConn.reset();
}

void RosterModel::onAccountConnectionChanged(const Tp::ConnectionPtr &conn) {
    qDebug() << "account connection changed";
    if (conn) {
        setConnection(conn);
    } else {
        unsetConnection();
    }
}

RosterElement * RosterModel::createItemForContact(const Tp::ContactPtr &contact, bool &exists) {

    Q_ASSERT(!contact.isNull());

    RosterElement * item;
    exists = false;
    for (int i = 0; i < mList.count(); ++i) {
        item = mList.at(i);
        if (item->contact() == contact) {
            exists = true;
            return item;
        }
    }

    //return new RosterElement(contact, mList);
    return new RosterElement(contact, this);
}


void RosterModel::onContactManagerStateChanged(Tp::ContactListState state) {

    Q_ASSERT(!mConn.isNull());

    // check if the contact manager has received a contact list, and populate the model if it has
    if (state == Tp::ContactListStateSuccess) {
        qDebug() << "Loading contacts";
        foreach (const Tp::ContactPtr &contact, mConn->contactManager()->allKnownContacts()) {
            addContactToModel(contact);
        }
    } else {
        qDebug() << "Contact list not retrieved, can't load contacts";
    }
}


void RosterModel::onPresencePublicationRequested(const Tp::Contacts &contacts) {
    // when someone adds us as a contact, check if the contact is already in the list and add a new RosterElement for the contact if not
    qDebug() << "Presence publication requested";
    foreach (const Tp::ContactPtr &contact, contacts) {
        addContactToModel(contact);
    }
}


void RosterModel::onContactRetrieved(Tp::PendingOperation *op) {
    qDebug() << "Contacts received from request started in addContact";

    Tp::PendingContacts *pcontacts = qobject_cast<Tp::PendingContacts *>(op);

    // get list of all contacts
    QList<Tp::ContactPtr> contacts = pcontacts->contacts();

    Q_ASSERT(pcontacts->identifiers().size() == 1);

    QString username = pcontacts->identifiers().first();

    // if there is more than one contact in the list of contacts, or the first contact pointer is not valid, print error and return
    if (contacts.size() !=1 || !contacts.first()) {
        qWarning() << "Unable to add contact: " << username;
        return;
    }

    Tp::ContactPtr contact = contacts.first();
    qDebug() << "Requesting presence subscription to " << contact->id();

    // request presence subscription and wait for subscription state to change before continuing - if the pub and sub states are both "no", the contact will not be added to the roster model
    QEventLoop pause;
    connect(contact.data(), SIGNAL(subscriptionStateChanged(Tp::Contact::PresenceState)),
            &pause, SLOT(quit())
            );
    contact->requestPresenceSubscription();
    pause.exec(); // wait for subscription state to change before continuing

    addContactToModel(contact);
}


void RosterModel::setAccount(Tp::AccountPtr account) {

    Q_ASSERT(!account.isNull());

    qDebug() << "roster model setAccount called, setting account to: " << account->displayName();
    mAccount = account;

    // when account connection changes, refresh data
    connect(mAccount.data(),
            SIGNAL(connectionChanged(Tp::ConnectionPtr)),
            SLOT(onAccountConnectionChanged(Tp::ConnectionPtr)));

    // refresh all data
    onAccountConnectionChanged(mAccount->connection());

}

void RosterModel::onKnownContactsChanged(Tp::Contacts contactsAdded,Tp::Contacts contactsRemoved,Tp::Channel::GroupMemberChangeDetails details) {
    Q_UNUSED(details);

    qDebug() << "onKnownContactsChanged called, adding " << contactsAdded.size() << " contacts, removing " << contactsRemoved.size() << " contacts.";

    // add new contacts
    foreach (const Tp::ContactPtr &contact, contactsAdded) {
        addContactToModel(contact);
    }

    // remove contacts by comparing contact IDs
    foreach (const Tp::ContactPtr &contact, contactsRemoved) {

        QString name = contact->id();
        qDebug() << "searching for contact " << name;

        for( int i=0; i<mList.count(); ++i ) {
            if ( mList.at(i)->contactID() == name ) {
                qDebug() << "found contact, deleting";
                RosterElement * item = mList.at(i);
                beginRemoveRows(QModelIndex(), i, i);
                mList.takeAt(i);
                endRemoveRows();
                delete item;
            }
        }
    }

}


void RosterModel::addContactToModel(const Tp::ContactPtr &contact) {

    Q_ASSERT(!contact.isNull());

    // if the contact is not blocked, and we neither subscribe to or publish presence for that contact, they have probably been removed and we don't want to see them in the roster
    if (contact->publishState() == Tp::Contact::PresenceStateNo && contact->subscriptionState() == Tp::Contact::PresenceStateNo && !contact->isBlocked()) {
        qDebug() << "Contact " << contact->id() << " will not be added to the roster because they are not blocked and there is no presence subscription or publication";
    } else {
        bool exists = false;
        RosterElement *item = createItemForContact(contact, exists);
        if(exists) {
            qDebug() << "Contact " << item->contactID() << " already exists";
        } else {
            qDebug() << "Adding contact " << item->contactID();
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            mList.append(item);
            endInsertRows();
        }
    }
}



void RosterModel::addContact(QString JID) {
    qDebug() << "Adding contact with Jabber ID: " << JID;

    Q_ASSERT(!mConn.isNull());

    Tp::PendingContacts *pcontacts = mConn->contactManager()->contactsForIdentifiers(QStringList() << JID);
    connect(pcontacts,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onContactRetrieved(Tp::PendingOperation*))
            );
}


// QAbstractListModel functions

int RosterModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent); // Q_UNUSED disables unused variable warnings
    return mList.size();
}

QHash<int, QByteArray> RosterModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[ContactRole] = "contact";

    return roles;
}

QVariant RosterModel::data(const QModelIndex &index, int role) const {

    if (role == ContactRole) {
        return QVariant::fromValue(mList.at(index.row()));
    }

    return QVariant();
}

bool RosterModel::connected() const {
    if (mConn) {
        return true;
    }
    return false;
}
