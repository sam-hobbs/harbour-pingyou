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

public:
    RosterElement (Tp::ContactPtr contact, QObject *parent = 0);
    ~RosterElement();

    Tp::ContactPtr contact() const { return mContact; }

    QString contactID() const;
    QString status() const;

signals:
    void contactIDChanged();
    void statusChanged();

private Q_SLOTS:
    void onContactChanged();

private:
    Tp::ContactPtr mContact;
    QString mStatus;
};


// ==============================================

class RosterModel : public QAbstractListModel
{

    Q_OBJECT

public:
    RosterModel(QObject *parent = 0);
    //virtual ~RosterModel();
    ~RosterModel();

    //Q_INVOKABLE void setAccountPath(Tp::AccountPtr) const;
    //Q_INVOKABLE void setAccount(Tp::AccountPtr) const;


    Tp::ConnectionPtr connection() const {return mConn; }
    void setConnection(const Tp::ConnectionPtr &conn);
    void unsetConnection();
    //void onAccountConnectionChanged(const Tp::ConnectionPtr &conn);

    // rowCount() and data() are required functions for subclassing QAbstractListModel
    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    enum Columns {
        ContactRole = Qt::UserRole + 1,
    };

    Q_ENUMS(Columns)

public Q_SLOTS:
    Q_INVOKABLE void setAccount(Tp::AccountPtr);
    void onAccountConnectionChanged(const Tp::ConnectionPtr &conn);

protected:
    virtual RosterElement *createItemForContact(const Tp::ContactPtr &contact, bool &exists);

    QHash<int, QByteArray> roleNames() const; // exposes role names so they can be accessed via QML

private Q_SLOTS:
    void onContactManagerStateChanged(Tp::ContactListState state);
    void onPresencePublicationRequested(const Tp::Contacts &);
    void onContactRetrieved(Tp::PendingOperation *op);

private:
    QList<RosterElement *> mList;
    Tp::ConnectionPtr mConn;
    //QString mAccountPath; // holds account name for account whose roster data we are currently viewing
    Tp::AccountPtr mAccount; // shared pointer to account whose roster we are viewing
};

#endif
