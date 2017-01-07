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

        contentHeight: validDetail.height + enabledDetail.height + connectionManagerDetail.height + protocolDetail.height + displayDetail.height + nicknameDetail.height + connectsAutomaticallyDetail.height + changingPresenceDetail.height + automaticPresenceDetail.height + currentPresenceDetail.height + requestedPresenceDetail.height + connectionStatusDetail.height + connectionDetail.height

        Column {
            width: parent.width
            anchors.leftMargin: Theme.paddingLarge
            anchors.rightMargin: Theme.paddingLarge

            DetailItem {
                id: validDetail
                label: "Valid"
                //value: valid ? "true" : "false"
                value: account.valid ? "true" : "false"
            }

            DetailItem {
                id: enabledDetail
                label: "Enabled"
                //value: enabled ? "true" : "false"
                value: account.enabled ? "true" : "false"
            }

            DetailItem {
                id: connectionManagerDetail
                label: "Connection Manager"
                //value: connection_manager
                value: account.cmName
            }

            DetailItem {
                id: protocolDetail
                label: "Protocol Name"
                //value: protocol
                value: account.protocolName
            }

            // http://www.qtcentre.org/threads/48479-QAbstractListModel-does-not-update-QML-ListView

            DetailItem {
                id: displayDetail
                label: "Display Name"
                //value: display_name
                value: account.displayName
            }

            DetailItem {
                id: nicknameDetail
                label: "Nickname"
                //value: nickname
                value: account.nickname
            }

            DetailItem {
                id: connectsAutomaticallyDetail
                label: "Connects Automatically"
                //value: connects_automatically ? "true" : "false"
                value: account.connectsAutomatically ? "true" : "false"
            }

            DetailItem {
                id: changingPresenceDetail
                label: "Changing Presence"
                //value: changing_presence ? "true" : "false"
                value: account.changingPresence ? "true" : "false"
            }

            DetailItem {
                id: automaticPresenceDetail
                label: "Automatic Presence"
                //value: automatic_presence
                value: account.automaticPresence
            }

            DetailItem {
                id: currentPresenceDetail
                label: "Current Presence"
                //value:  current_presence
                value: account.automaticPresence
            }

            DetailItem {
                id: requestedPresenceDetail
                label: "Requested Presence"
                //value: requested_presence
                value: account.requestedPresence
            }

            DetailItem {
                id: connectionStatusDetail
                label: "Connection Status"
                //value: connection_status
                value: account.connectionStatus
            }

            DetailItem {
                id: connectionDetail
                label: "Connection Path"
                //value: connection
                value: account.connection.objectpath
            }

        }

    }
}
