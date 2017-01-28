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

import QtQuick 2.2
import Sailfish.Silica 1.0

import "../delegates"

Page {
    id: page
    allowedOrientations: Orientation.All

    SilicaListView {

        PullDownMenu {

            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }

            MenuItem {
                text: qsTr("Roster")
                onClicked: pageStack.push(Qt.resolvedUrl("RosterPage.qml"))
                visible: (accountsModel.numValidAccounts > 0)
            }

            MenuItem {
                text: qsTr("Create account")
                onClicked: pageStack.push(Qt.resolvedUrl("../dialogs/AccountPropertiesDialog.qml"),{parameterModel: {}, existingAccount: false},PageStackAction.Immediate)
            }
        }

        id: listView
        anchors.fill: parent
        width: parent.width
        header: PageHeader {
            title: "Accounts"
        }

        model: accountsModel
        delegate: AccountEntry {}

        VerticalScrollDecorator {}

    }

}
