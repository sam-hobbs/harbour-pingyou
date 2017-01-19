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

BackgroundItem {
    id: delegate
    height: itemId.height + 10

    RemorseItem { id: remorse }

    function showRemorseItem() {
        var idx = index
        remorse.execute(delegate, "Deleting", function() {account.remove()})
    }

    ListItem {
        id: itemId
        width: parent.width

        menu: ContextMenu {
            MenuItem {
                text: qsTr("Roster")
                visible: (account.valid && account.enabled)
                onClicked: {
                    console.log("Loading roster for " + account.displayName)
                    account.setActiveAccount();
                    pageStack.push(Qt.resolvedUrl("../pages/RosterPage.qml"))
                }
            }

            MenuItem {
                text: qsTr("Account Settings")
                onClicked: {
                    console.log("Loading parameters page for " + account.displayName)
                    pageStack.push(Qt.resolvedUrl("../dialogs/AccountPropertiesDialog.qml"),{parameterModel: account.parameterList},PageStackAction.Immediate)
                }
            }

            MenuItem {
                text: account.enabled ? qsTr("Disable") : qsTr("Enable")
                onClicked: {
                    console.log("Toggling enabled state for " + account.displayName)
                    account.toggleEnabled();
                }
            }

            MenuItem {
                text: qsTr("Remove account")
                onClicked: {
                    console.log("Removing account " + account.displayName)
                    delegate.showRemorseItem()
                }
            }
        }

        contentHeight: expanding.height

        ExpandingSection {
            id: expanding
            title: (account.valid && account.enabled) ? account.nickname + " (enabled)" : account.nickname + " (disabled)"
            expanded: (account.valid && account.enabled)
            //height: column.height
            content.sourceComponent:

            Column {
                id: column

                width: parent.width
                anchors.leftMargin: Theme.paddingLarge
                anchors.rightMargin: Theme.paddingLarge
                //height: avatarImage.height + nicknameDetail.height + currentPresenceDetail.height + devInfo.height

                Image {
                    id: avatarImage
                    width: parent.width
                    asynchronous: true // don't block the UI while the image is loaded
                    fillMode: Image.PreserveAspectFit
                    source: account.avatarPath

                }

                DetailItem {
                    id: validDetail
                    label: "Valid"
                    value: account.valid ? "true" : "false"
                    //visible: settings.displayDevInfo
                }

                DetailItem {
                    id: enabledDetail
                    label: "Enabled"
                    value: account.enabled ? "true" : "false"
                    //visible: settings.displayDevInfo
                }

                DetailItem {
                    id: displayNameDetail
                    label: "Display name"
                    value: account.displayName
                    visible: (settings.displayDevInfo || (account.displayName !== account.nickname))
                }

                DetailItem {
                    id: nicknameDetail
                    label: "Nickname"
                    value: account.nickname
                    visible: settings.displayDevInfo
                }

                DetailItem {
                    id: connectionManagerDetail
                    label: "Connection Manager"
                    value: account.cmName
                    visible: settings.displayDevInfo
                }

                DetailItem {
                    id: protocolDetail
                    label: "Protocol Name"
                    value: account.protocolName
                    visible: settings.displayDevInfo
                }

                DetailItem {
                    id: connectsAutomaticallyDetail
                    label: "Connects Automatically"
                    value: account.connectsAutomatically ? "true" : "false"
                    visible: settings.displayDevInfo
                }

                DetailItem {
                    id: currentPresenceDetail
                    label: "Current Presence"
                    value: account.automaticPresence
                }

                DetailItem {
                    id: changingPresenceDetail
                    label: "Changing Presence"
                    value: account.changingPresence ? "true" : "false"
                    visible: settings.displayDevInfo
                }

                DetailItem {
                    id: automaticPresenceDetail
                    label: "Automatic Presence"
                    value: account.automaticPresence
                    visible: settings.displayDevInfo
                }

                DetailItem {
                    id: requestedPresenceDetail
                    label: "Requested Presence"
                    value: account.requestedPresence
                    visible: settings.displayDevInfo
                }

                DetailItem {
                    id: connectionStatusDetail
                    label: "Connection Status"
                    value: account.connectionStatus
                    visible: settings.displayDevInfo
                }

                DetailItem {
                    id: connectionDetail
                    label: "Connection Path"
                    value: account.connectionPath
                    visible: settings.displayDevInfo
                }

            }
        }

        // shade the items so that it is easy to distinguish beteween accounts
        Item {
            anchors.fill: parent

            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: parent.height

                gradient: Gradient {
                    GradientStop { position: 0.0; color: Theme.rgba(Theme.primaryColor, 0) }
                    GradientStop { position: 1.0; color: Theme.rgba(Theme.primaryColor, 0.1) }
                }
            }
        }

    }
}
