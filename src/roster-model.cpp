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
#include <TelepathyQt/ContactManager>
#include <TelepathyQt/PendingConnection>
#include <TelepathyQt/PendingContacts>
#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/Presence>
#include <QObject>
#include <QDebug>


#include "roster-model.h"

RosterElement::RosterElement (Tp::ContactPtr contact, QObject *parent) : QObject(parent), mContact(contact) {
    onContactChanged();

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

}

RosterElement::~RosterElement() {

}


void RosterElement::onContactChanged() {

//    QString status = mContact->presence().status();

//    // if I've asked to see the contact presence
//    if (mContact->subscriptionState() == Tp::Contact::PresenceStateAsk) {
//        mStatus = status + " (awaiting approval)";
//    }

//    // if the contact asked to see my presence

//    else if (mContact->publishState() == Tp::Contact::PresenceStateAsk) {
//        mStatus = status + " (pending approval)";
//    }

//    else if (mContact->subscriptionState() == Tp::Contact::PresenceStateNo && mContact->publishState() == Tp::Contact::PresenceStateNo) {
//        mStatus = status + " (unknown)";
//    }

//    else {
//        mStatus = status;
//    }

//    // if the contact is blocked, override the status to blocked
//    if (mContact->isBlocked()) {
//        mStatus = status + " (blocked)";
//    }

    mStatus = mContact->presence().status();
    emit statusChanged();

}

QString RosterElement::contactID() const {
    return mContact->id();
}

QString RosterElement::status() const {
    return mStatus;
}

void RosterElement::authPubAction() {
    qDebug() << "Auth pub action triggered";
    if (mContact->publishState() != Tp::Contact::PresenceStateYes )
        mContact->authorizePresencePublication();
}

void RosterElement::denyPubAction() {
    qDebug() << "Deny pub action triggered";
    if (mContact->publishState() != Tp::Contact::PresenceStateNo)
        mContact->removePresencePublication();
}

void RosterElement::removePubAction() {
    qDebug() << "Remove pub action triggered";
    if(mContact->subscriptionState() != Tp::Contact::PresenceStateNo) {
        mContact->removePresencePublication();
        mContact->removePresenceSubscription();
    }
}

void RosterElement::toggleBlockAction() {
    qDebug() << "toggle block action triggered";
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
    if(mContact->subscriptionState() == Tp::Contact::PresenceStateNo) {
        mContact->requestPresenceSubscription();
    }
}


void RosterElement::rescindSubRequestAction() {
    qDebug() << "rescind sub request action triggered";
    if(mContact->subscriptionState() == Tp::Contact::PresenceStateAsk) {
        mContact->removePresenceSubscription();
    }
}






QString RosterElement::publishState() const {

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
    return mContact->isBlocked();
}


bool RosterElement::canAuthorisePublication() const {
    return mContact->manager()->canAuthorizePresencePublication();
}

bool RosterElement::canRemovePublication() const {
    return mContact->manager()->canRemovePresencePublication();
}

bool RosterElement::canBlockContacts() const {
    return mContact->manager()->canBlockContacts();
}

bool RosterElement::canRequestPresenceSubscription() const {
    return mContact->manager()->canRequestPresenceSubscription();
}

bool RosterElement::canRescindPresenceSubscriptionRequest() const {
    return mContact->manager()->canRescindPresenceSubscriptionRequest();
}

// =============================================================

RosterModel::RosterModel(QObject *parent) {
    Q_UNUSED(parent);
}

RosterModel::~RosterModel() {

}

void RosterModel::setConnection(const Tp::ConnectionPtr &conn) {

    qDebug() << "setConnection called";

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
            SLOT(refreshRoster())
            );

    connect(conn->contactManager().data(),
            SIGNAL(stateChanged(Tp::ContactListState)),
            SLOT(onContactManagerStateChanged(Tp::ContactListState)));

    onContactManagerStateChanged(conn->contactManager()->state());

}


void RosterModel::unsetConnection() {

    qDebug() << "unsetConnection called";

    // remove existing RosterElements from the list and then delete them
    while (mList.count() > 0) {
        RosterElement * item = mList.at(0); // get pointer to current item for deletion later
        mList.takeAt(0);
        delete item;
    }
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

    // check if the contact manager has received a contact list, and populate the model if it has

    if (state == Tp::ContactListStateSuccess) {
        qDebug() << "Loading contacts";
        RosterElement *item;
        bool exists;
        foreach (const Tp::ContactPtr &contact, mConn->contactManager()->allKnownContacts()) {
            exists = false;
            item = createItemForContact(contact, exists);
            if(!exists) {
                // connect signals and slots?
                mList.append(item);
            }
        }
    } else {
        qDebug() << "Contact list not retrieved, not loading contacts";
    }
}


void RosterModel::onPresencePublicationRequested(const Tp::Contacts &contacts) {

    // when someone adds us as a contact, check if the contact is already in the list and add a new RosterElement for the contact if not

    qDebug() << "Presence publication requested";
    RosterElement *item;
    bool exists;
    foreach (const Tp::ContactPtr &contact, contacts) {
        exists = false;
        item = createItemForContact(contact, exists);
        if(!exists) {
            // connect signals and slots?
            mList.append(item);
        }
    }
}


void RosterModel::onContactRetrieved(Tp::PendingOperation *op) {

    Tp::PendingContacts *pcontacts = qobject_cast<Tp::PendingContacts *>(op);

    // get list of all contacts
    QList<Tp::ContactPtr> contacts = pcontacts->contacts();

    Q_ASSERT(pcontacts->identifiers().size() == 1);

    QString username = pcontacts->identifiers().first();

    if (contacts.size() !=1 || contacts.first()) {
        qWarning() << "Unable to add contact: " << username;
        return;
    }

    Tp::ContactPtr contact = contacts.first();
    qDebug() << "Request presence subscription for contact " << username;
    bool exists = false;
    RosterElement *item = createItemForContact(contact,exists);
    if(!exists) {
        // connect signals and slots?
        mList.append(item);
    }
    contact->requestPresenceSubscription();

}


void RosterModel::setAccount(Tp::AccountPtr account) {
    qDebug() << "roster model setAccount called, setting account to: " << account->displayName();
    mAccount = account;

    // when account connection changes, refresh data
    connect(mAccount.data(),
            SIGNAL(connectionChanged(Tp::ConnectionPtr)),
            SLOT(onAccountConnectionChanged(Tp::ConnectionPtr)));

    // refresh all data
    onAccountConnectionChanged(mAccount->connection());

}

void RosterModel::refreshRoster() {
    // force a refresh of the roster
    qDebug() << "Refresh roster called";
    unsetConnection(); // clear the model
    onContactManagerStateChanged(mConn->contactManager()->state()); // refresh the model using the existing connection
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
