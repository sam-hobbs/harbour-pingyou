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

#include "accounts-model.h"

// https://doc.qt.io/qt-5/qtquick-modelviewsdata-modelview.html
// https://doc.qt.io/qt-5/qtquick-modelviewsdata-cppmodels.html


namespace Tp {
class AccountManager;
class PendingOperation;
}




class AccountElement
{
public:
    AccountElement (Tp::AccountPtr acc, QObject *parent = 0);
    Tp::AccountPtr mAcc; // needs to be public so that accounts model can access the data
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
        ColumnValid = Qt::UserRole + 1,
        ColumnEnabled,
        ColumnConnectionManager,
        ColumnProtocol,
        ColumnDisplayName,
        ColumnNickname,
        ColumnConnectsAutomatically,
        ColumnChangingPresence,
        ColumnAutomaticPresence,
        ColumnCurrentPresence,
        ColumnRequestedPresence,
        ColumnConnectionStatus,
        ColumnConnection,
    };

    Q_ENUMS(Columns)

    void addAccountElement(const Tp::AccountPtr &);

protected:
    QHash<int, QByteArray> roleNames() const; // exposes role names so they can be accessed via QML

private Q_SLOTS:
    void onAMReady(Tp::PendingOperation *);
    //void onNewAccount(const Tp::AccountPtr &);

private:
    Tp::AccountManagerPtr mAM;
    QList<AccountElement *> myList; // could make this a QList of AccountPtr's?
};

#endif
