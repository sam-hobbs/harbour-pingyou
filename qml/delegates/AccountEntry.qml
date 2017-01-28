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
                    pageStack.push(Qt.resolvedUrl("../dialogs/AccountPropertiesDialog.qml"),{parameterModel: account.parameterList, existingAccount: true},PageStackAction.Immediate)
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

            MenuItem {
                text: account.connectsAutomatically ? qsTr("Don't connect automatically") : qsTr("Connect automatically")
                onClicked: {
                    account.connectsAutomatically = !account.connectsAutomatically
                }
            }

            MenuItem {
                text: qsTr("Reconnect")
                onClicked: account.reconnect()
            }
        }

        contentHeight: expanding.height

        ExpandingSection {
            id: expanding
            //title: (account.valid && account.enabled) ? account.nickname + " (enabled)" : account.nickname + " (disabled)"
            title: (account.nickname !== "") ? account.nickname : account.displayName
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
                    height: (avatarImage.status === Image.Ready) ? width : 0
                    asynchronous: true // don't block the UI while the image is loaded
                    fillMode: Image.PreserveAspectFit
                    source: account.avatarPath
                    width: parent.width/2
                    anchors.horizontalCenter: parent.horizontalCenter
                    //cache: false
                }

                Item {
                    width: 1
                    //height: Theme.paddingMedium
                    height: Theme.paddingLarge
                    visible: (avatarImage.height > 0) ? true : false
                }

                Rectangle {
                    width: parent.width/2
                    height: Theme.paddingMedium
                    color: (account.connectionError === "") ?  account.online ? "green" : "grey" : "red"
                    radius: 10
                    anchors.horizontalCenter: parent.horizontalCenter
                }


                ComboBox {
                    width: parent.width
                    label: "Presence"
                    menu: ContextMenu {
                        MenuItem {
                            text: "Offline"
                            onClicked: account.setRequestedPresence(1,"offline")
                        }

                        MenuItem {
                            text: "Available"
                            onClicked: account.setRequestedPresence(2,"available")
                        }

                        MenuItem {
                            text: "Away"
                            onClicked: account.setRequestedPresence(3,"away")
                        }

                        MenuItem {
                            text: "Extended Away"
                            onClicked: account.setRequestedPresence(4,"extended away")
                        }

                        MenuItem {
                            text: "Hidden"
                            onClicked: account.setRequestedPresence(5,"hidden")
                        }

                        MenuItem {
                            text: "Busy"
                            onClicked: account.setRequestedPresence(6,"busy")
                        }
                    }

                    currentIndex: {
                        switch(account.automaticPresence) {
                        case "offline": 0; break;
                        case "available": 1; break;
                        case "away": 2; break;
                        case "extended away": 3; break;
                        case "hidden": 4; break;
                        case "busy": 5; break;
                        default: -1; break;
                        }
                    }
                }

                // https://telepathy.freedesktop.org/spec/Connection_Interface_Simple_Presence.html#Enum:Connection_Presence_Type
                ComboBox {
                    width: parent.width
                    label: "Automatic presence"

                    currentIndex: {
                        switch(account.automaticPresence) {
                        case "available": 0; break;
                        case "away": 1; break;
                        case "extended away": 2; break;
                        case "hidden": 3; break;
                        case "busy": 4; break;
                        default: -1; break;
                        }
                    }

                    menu: ContextMenu {

                        MenuItem {
                            text: "Available"
                            onClicked: account.setAutomaticPresence(2,"available")
                        }

                        MenuItem {
                            text: "Away"
                            onClicked: account.setAutomaticPresence(3,"away")
                        }

                        MenuItem {
                            text: "Extended Away"
                            onClicked: account.setAutomaticPresence(4,"extended away")
                        }

                        MenuItem {
                            text: "Hidden"
                            onClicked: account.setAutomaticPresence(5,"hidden")
                        }

                        MenuItem {
                            text: "Busy"
                            onClicked: account.setAutomaticPresence(6,"busy")
                        }
                    }
                }



                DetailItem {
                    id: onlineDetail
                    label: "Online"
                    value: account.online ? "true" : "false"
                    //visible: settings.displayDevInfo
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
                    id: normalisedAccountNameDetail
                    label: "Normalised account name"
                    value: account.normalisedName
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
                    value: {
                        switch(account.connectionStatus) {
                        case 0: "Connected"; break;
                        case 1: "Connecting"; break;
                        case 2: "Disconnected"; break;
                        default: ""; break;
                        }
                    }
                    visible: settings.displayDevInfo
                }

                // https://telepathy.freedesktop.org/spec/Connection.html#Enum:Connection_Status
                DetailItem {
                    id: connectionStatusReasonDetail
                    label: "Connection status reason"
                    value: {
                        switch(account.connectionStatusReason) {
                        case 0: "None specified"; break;
                        case 1: "Requested"; break;
                        case 2: "Network error"; break;
                        case 3: "Authentication failed"; break;
                        case 4: "Encryption error"; break;
                        case 5: "Name in use"; break;
                        case 6: "Cert not provided"; break;
                        case 7: "Cert untrusted"; break;
                        case 8: "Cert expired"; break;
                        case 9: "Cert not activated"; break;
                        case 10: "Cert hostname mismatch"; break;
                        case 11: "Cert fingerprint mismatch"; break;
                        case 12: "Cert self signed"; break;
                        case 13: "Cert other error"; break;
                        case 14: "Cert revoked"; break;
                        case 15: "Cert insecure"; break;
                        case 16: "Cert limit exceeded"; break;
                        default: ""; break;
                        }
                    }
                    visible: settings.displayDevInfo
                }


                Column {
                    id: connectionError
                    width: parent.width
                    visible: (account.connectionError === "") ? false : true

                    Label {
                        text: "Connection Error"

                        anchors.horizontalCenter: parent.horizontalCenter

                        font.family: Theme.fontFamilyHeading
                    }

                    Image {
                        asynchronous: true
                        source: "image://theme/icon-lock-warning"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Label {
                        text: account.connectionError

                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter

                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        width: parent.width

                        font.family: Theme.fontFamilyHeading
                        font.pixelSize: Theme.fontSizeSmall

                    }

                    Label {
                        text: account.connectionErrorDetails["debug-message"] ? account.connectionErrorDetails["debug-message"] : ""

                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter

                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        width: parent.width

                        font.pixelSize: Theme.fontSizeSmall
                    }
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
