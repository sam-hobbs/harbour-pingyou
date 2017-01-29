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

#ifndef PINGYOU_ROSTER_MODEL_H
#define PINGYOU_ROSTER_MODEL_H

#include <TelepathyQt/Account>
#include <TelepathyQt/Contact>
#include <TelepathyQt/Connection>
#include <QAbstractListModel>


namespace Tp {
class Connection;
class PendingOperation;
}


// ==============================================

class RosterElement : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString contactID READ contactID NOTIFY contactIDChanged) // constant ?
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

    Q_PROPERTY(QString publishState READ publishState NOTIFY publishStateChanged)
    Q_PROPERTY(QString subscriptionState READ subscriptionState NOTIFY subscriptionStateChanged)
    Q_PROPERTY(bool blocked READ blocked NOTIFY blockedChanged)

    Q_PROPERTY(QString avatarPath READ avatarPath NOTIFY avatarPathChanged)

    // these properties hold information about the capabilities of the contact manager, used to decide which actions are enabled
    Q_PROPERTY(bool canAuthorisePublication READ canAuthorisePublication CONSTANT)
    Q_PROPERTY(bool canRemovePublication READ canRemovePublication CONSTANT)
    Q_PROPERTY(bool canBlockContacts READ canBlockContacts CONSTANT)
    Q_PROPERTY(bool canRequestPresenceSubscription READ canRequestPresenceSubscription CONSTANT)
    Q_PROPERTY(bool canRescindPresenceSubscriptionRequest READ canRescindPresenceSubscriptionRequest CONSTANT)
    

public:
    RosterElement (Tp::ContactPtr contact, QObject *parent = 0);
    ~RosterElement();

    Tp::ContactPtr contact() const { return mContact; }

    QString contactID() const;
    QString status() const;

    QString publishState() const;
    QString subscriptionState() const;
    bool blocked() const;

    bool canAuthorisePublication() const;
    bool canRemovePublication() const;
    bool canBlockContacts() const;
    bool canRequestPresencePublication() const;
    bool canRequestPresenceSubscription() const;
    bool canRescindPresenceSubscriptionRequest() const;

    QString avatarPath() const;

    Q_INVOKABLE void authPubAction(); // authorise presence publication
    Q_INVOKABLE void denyPubAction(); // deny presence publication
    Q_INVOKABLE void removePubAction(); // remove presence publication and subscription
    Q_INVOKABLE void toggleBlockAction(); // block or unblock contact
    Q_INVOKABLE void requestSubAction(); // request presence subscription from this contact
    Q_INVOKABLE void rescindSubRequestAction(); // cancel presence subscription request

signals:
    void contactIDChanged();
    void statusChanged();

    void publishStateChanged();
    void subscriptionStateChanged();
    void blockedChanged();

    void avatarPathChanged(QString);

private Q_SLOTS:
    void onContactChanged();
    void onAvatarTokenChanged(QString);
    void onAvatarDataChanged(Tp::AvatarData);

private:
    Tp::ContactPtr mContact;
    QString mStatus;
    QString mAvatarToken;
};


// ==============================================

class RosterModel : public QAbstractListModel
{

    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

public:
    RosterModel(QObject *parent = 0);
    //virtual ~RosterModel();
    ~RosterModel();

    Tp::ConnectionPtr connection() const {return mConn; }
    void setConnection(const Tp::ConnectionPtr &conn);
    void unsetConnection();

    bool connected() const;

    // rowCount() and data() are required functions for subclassing QAbstractListModel
    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    enum Columns {
        ContactRole = Qt::UserRole + 1,
    };

    Q_ENUMS(Columns)

    Q_INVOKABLE void addContact(QString);

signals:
    void connectedChanged();
public Q_SLOTS:
    Q_INVOKABLE void setAccount(Tp::AccountPtr);
    void onAccountConnectionChanged(const Tp::ConnectionPtr &conn);
    void onKnownContactsChanged(Tp::Contacts,Tp::Contacts,Tp::Channel::GroupMemberChangeDetails);

protected:
    virtual RosterElement *createItemForContact(const Tp::ContactPtr &contact, bool &exists);

    QHash<int, QByteArray> roleNames() const; // exposes role names so they can be accessed via QML

private Q_SLOTS:
    void onContactManagerStateChanged(Tp::ContactListState state);
    void onPresencePublicationRequested(const Tp::Contacts &);
    void onContactRetrieved(Tp::PendingOperation *op);

private:
    void addContactToModel(const Tp::ContactPtr &);

    QList<RosterElement *> mList;
    Tp::ConnectionPtr mConn;
    //QString mAccountPath; // holds account name for account whose roster data we are currently viewing
    Tp::AccountPtr mAccount; // shared pointer to account whose roster we are viewing
};

#endif
