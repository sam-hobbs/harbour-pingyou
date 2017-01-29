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

#ifndef PINGYOU_ACCOUNTS_MODEL_H
#define PINGYOU_ACCOUNTS_MODEL_H

#include <TelepathyQt/AccountManager>
#include <TelepathyQt/Types>
#include <QObject>
#include <QAbstractListModel>
#include <TelepathyQt/AvatarData>
#include <QTemporaryFile>

namespace Tp {
class AccountManager;
class PendingOperation;
}


class AccountElement : public QObject
{
    Q_OBJECT

public:
    // properties of account recreated here
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QString cmName READ cmName CONSTANT) // not notifiable
    Q_PROPERTY(QString protocolName READ protocolName CONSTANT) // not notifiable
    Q_PROPERTY(QString displayName READ displayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString nickname READ nickname NOTIFY nicknameChanged)
    Q_PROPERTY(bool connectsAutomatically READ connectsAutomatically WRITE setConnectsAutomatically NOTIFY connectsAutomaticallyChanged)
    Q_PROPERTY(bool changingPresence READ changingPresence NOTIFY changingPresenceChanged)

    Q_PROPERTY(QString automaticPresence READ automaticPresence NOTIFY automaticPresenceChanged)
    Q_PROPERTY(QString currentPresence READ currentPresence NOTIFY currentPresenceChanged)
    Q_PROPERTY(QString requestedPresence READ requestedPresence NOTIFY requestedPresenceChanged)

    Q_PROPERTY(QVariant connectionStatus READ connectionStatus NOTIFY connectionStatusChanged)
    Q_PROPERTY(QVariant connectionStatusReason READ connectionStatusReason NOTIFY connectionStatusReasonChanged)
    Q_PROPERTY(QString connectionPath READ connectionPath NOTIFY connectionPathChanged)
    Q_PROPERTY(QString normalisedName READ normalisedName NOTIFY normalisedNameChanged)

    Q_PROPERTY(bool online READ online NOTIFY onlineChanged)

    Q_PROPERTY(QString avatarPath READ avatarPath NOTIFY avatarPathChanged)

    Q_PROPERTY(QVariant parameterList READ parameterList NOTIFY parameterListChanged)

    Q_PROPERTY(QString connectionError READ connectionError NOTIFY connectionStatusChanged)
    Q_PROPERTY(QVariant connectionErrorDetails READ connectionErrorDetails NOTIFY connectionStatusChanged)

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
    Tp::ConnectionStatusReason connectionStatusReason() const;

    QString connectionPath() const;
    QString normalisedName() const;

    bool online() const;

    QString connectionError() const;
    QVariant connectionErrorDetails() const;

    QVariant parameterList() const;

    QString avatarPath() const;

    Tp::AccountPtr getAccountPtr();

    Q_INVOKABLE void setActiveAccount();

    Q_INVOKABLE void remove();

    Q_INVOKABLE void toggleEnabled();

    void setConnectsAutomatically(bool);
    void setEnabled(bool);

    Q_INVOKABLE void reconnect();

    Q_INVOKABLE void setAutomaticPresence(int, QString statusString);
    Q_INVOKABLE void setRequestedPresence(int, QString statusString);

    Q_INVOKABLE void setAvatar(QString path);

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
    void connectionStatusReasonChanged();

    void connectionPathChanged(Tp::ConnectionPtr);

    void onlineChanged(bool);

    void avatarPathChanged(QString);

    void setThisAccountActive(Tp::AccountPtr);

    void parameterListChanged();

    void normalisedNameChanged(QString);

    void deleteMe(AccountElement *);

public slots:

    void avatarChanged(Tp::Avatar);

private Q_SLOTS:
    void accountRemovalResultHandler(Tp::PendingOperation*);

private:
    Tp::AccountPtr mAcc;
    QTemporaryFile *mAvatarFile;
};



// ==========================================================================


class AccountsModel : public QAbstractListModel
{

    Q_OBJECT
    Q_PROPERTY( int numValidAccounts READ numValidAccounts NOTIFY numValidAccountsChanged)
    Q_PROPERTY( int numOnlineAccounts READ numOnlineAccounts NOTIFY numOnlineAccountsChanged)

public:
    AccountsModel(QObject *parent = 0);

    // rowCount() and data() are required functions for subclassing QAbstractListModel
    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    enum Columns {
        AccountRole = Qt::UserRole + 1,
    };

    Q_ENUMS(Columns)

    int numValidAccounts() const;
    int numOnlineAccounts() const;

    Q_INVOKABLE void prepareParameterQVariantMap(QVariantMap);
private:
    void createAccount(QVariantMap);
    void changeAccountParameters(Tp::AccountPtr,QVariantMap);

signals:
    void newAccountPtr(Tp::AccountPtr);
    void numValidAccountsChanged();
    void numOnlineAccountsChanged();

public Q_SLOTS:
    void genericErrorHandler(Tp::PendingOperation *);

protected:
    QHash<int, QByteArray> roleNames() const; // exposes role names so they can be accessed via QML

private Q_SLOTS:
    void onAMReady(Tp::PendingOperation *);
    void addAccountElement(const Tp::AccountPtr &);
    void removeAccount(AccountElement *);
    void onAccountCreationFinished(Tp::PendingOperation *);

private:
    Tp::AccountManagerPtr mAM;
    QList<AccountElement *> myList;
};

#endif
