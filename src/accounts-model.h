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

#ifndef PINGYOU_ACCOUNTS_MODEL_H
#define PINGYOU_ACCOUNTS_MODEL_H

#include <TelepathyQt/AccountManager>
#include <TelepathyQt/Types>
#include <QObject>
#include <QAbstractListModel>
#include <TelepathyQt/AvatarData>

#include "accounts-model.h"

// https://doc.qt.io/qt-5/qtquick-modelviewsdata-modelview.html
// https://doc.qt.io/qt-5/qtquick-modelviewsdata-cppmodels.html


namespace Tp {
class AccountManager;
class PendingOperation;
}


// use qmlRegisterUncreatableType for this?

class AccountElement : public QObject
{
    Q_OBJECT

public:
    // properties of account recreated here
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
    Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged)
    Q_PROPERTY(QString cmName READ cmName CONSTANT) // not notifiable
    Q_PROPERTY(QString protocolName READ protocolName CONSTANT) // not notifiable
    Q_PROPERTY(QString displayName READ displayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString nickname READ enabled NOTIFY nicknameChanged)
    Q_PROPERTY(bool connectsAutomatically READ connectsAutomatically NOTIFY connectsAutomaticallyChanged)
    Q_PROPERTY(bool changingPresence READ changingPresence NOTIFY changingPresenceChanged)
    Q_PROPERTY(QString automaticPresence READ automaticPresence NOTIFY automaticPresenceChanged)
    Q_PROPERTY(QString currentPresence READ currentPresence NOTIFY currentPresenceChanged)
    Q_PROPERTY(QString requestedPresence READ requestedPresence NOTIFY requestedPresenceChanged)
    Q_PROPERTY(QVariant connectionStatus READ connectionStatus NOTIFY connectionStatusChanged)
    Q_PROPERTY(QString connectionPath READ connectionPath NOTIFY connectionPathChanged)

    Q_PROPERTY(bool online READ online NOTIFY onlineChanged)

    Q_PROPERTY(QString avatarPath READ avatarPath NOTIFY avatarPathChanged) //TODO

    AccountElement (Tp::AccountPtr acc, QObject *parent = 0);


    bool valid() const;
    bool enabled() const;
    QString cmName() const;
    QString protocolName() const;
    QString displayName() const;
    QString nickname() const;
    bool connectsAutomatically() const;
    bool changingPresence() const;
    QString automaticPresence() const;
    QString currentPresence() const;
    QString requestedPresence() const;
    Tp::ConnectionStatus connectionStatus() const;
    QString connectionPath() const;

    bool online() const;

    QString avatarPath() const;

signals:
    void validChanged(bool);
    void enabledChanged(bool);
    void cmNameChanged(QString);
    // cmName is not notifiable
    // protocolName is not notifiable
    void displayNameChanged(QString);
    void nicknameChanged(QString);
    void connectsAutomaticallyChanged(bool);
    void changingPresenceChanged(bool);

    void automaticPresenceChanged(Tp::Presence);
    void currentPresenceChanged(Tp::Presence);
    void requestedPresenceChanged(Tp::Presence);

    void connectionStatusChanged(Tp::ConnectionStatus);
    void connectionPathChanged(Tp::ConnectionPtr);

    void onlineChanged(bool);

    void avatarPathChanged(QString);

public slots:

    void avatarChanged(Tp::Avatar) const;

private:
    Tp::AccountPtr mAcc;
};






class AccountsModel : public QAbstractListModel
{

    Q_OBJECT

public:
    AccountsModel(QObject *parent = 0);

    // rowCount() and data() are required functions for subclassing QAbstractListModel
    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    enum Columns {
        AccountRole = Qt::UserRole + 1,
    };

    Q_ENUMS(Columns)

protected:
    QHash<int, QByteArray> roleNames() const; // exposes role names so they can be accessed via QML

private Q_SLOTS:
    void onAMReady(Tp::PendingOperation *);
    void addAccountElement(const Tp::AccountPtr &);

private:
    Tp::AccountManagerPtr mAM;
    QList<AccountElement *> myList;
};

#endif
