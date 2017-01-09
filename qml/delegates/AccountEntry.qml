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
    ListItem {
        id: itemId
        width: parent.width

        contentHeight: column.visible ? button.height + column.height : button.height

        Button {
            id: button
            anchors.left: parent.left
            anchors.right: parent.right

            text: (account.valid && account.enabled) ? account.displayName + " (enabled)" : account.displayName + " (disabled)"
            onClicked: {column.visible = !column.visible}

        }

        Column {
            id: column
            visible: (account.enabled && account.enabled)

            width: parent.width
            anchors.top: button.bottom
            anchors.leftMargin: Theme.paddingLarge
            anchors.rightMargin: Theme.paddingLarge

//            Label {
//                id: title
//                width: parent.width
//                text: account.displayName
//                font.pixelSize: Theme.fontSizeMedium
//                horizontalAlignment: Text.AlignHCenter
//            }

            Image {
                id: avatarImage
                width: parent.width
                asynchronous: true // don't block the UI while the image is loaded
                fillMode: Image.PreserveAspectFit
                source: account.avatarPath

            }

//            TextSwitch {
//                id: enabledSwitch
//                text: "Enabled"
//                automaticCheck: false
//                checked: account.enabled
//            }

            DetailItem {
                id: validDetail
                label: "Valid"
                value: account.valid ? "true" : "false"
            }

            DetailItem {
                id: enabledDetail
                label: "Enabled"
                value: account.enabled ? "true" : "false"
            }

            DetailItem {
                id: connectionManagerDetail
                label: "Connection Manager"
                value: account.cmName
            }

            DetailItem {
                id: protocolDetail
                label: "Protocol Name"
                value: account.protocolName
            }

            DetailItem {
                id: nicknameDetail
                label: "Nickname"
                value: account.nickname
            }

            DetailItem {
                id: connectsAutomaticallyDetail
                label: "Connects Automatically"
                value: account.connectsAutomatically ? "true" : "false"
            }

            DetailItem {
                id: changingPresenceDetail
                label: "Changing Presence"
                value: account.changingPresence ? "true" : "false"
            }

            DetailItem {
                id: automaticPresenceDetail
                label: "Automatic Presence"
                value: account.automaticPresence
            }

            DetailItem {
                id: currentPresenceDetail
                label: "Current Presence"
                value: account.automaticPresence
            }

            DetailItem {
                id: requestedPresenceDetail
                label: "Requested Presence"
                value: account.requestedPresence
            }

            DetailItem {
                id: connectionStatusDetail
                label: "Connection Status"
                value: account.connectionStatus
            }

            DetailItem {
                id: connectionDetail
                label: "Connection Path"
                value: account.connectionPath
            }

        }

    }
}
