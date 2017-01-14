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
        contentHeight: column.height

        Column {
            id: column
            width: parent.width

            Label {
                id: contactID
                text: contact.contactID
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
            }

            DetailItem {
                id: statusDetail
                label: "Status"
                value: contact.status
            }

            DetailItem {
                id: publishDetail
                label: "Publish state"
                value: contact.publishState
            }

            DetailItem {
                id: subscriptionDetail
                label: "Subscription state"
                value: contact.subscriptionState
            }

            DetailItem {
                id: blockDetail
                label: "Block state"
                value: contact.blocked
            }




            DetailItem {
                id: canAuthorisePub
                label: "Can authorise publication"
                value: contact.canAuthorisePublication
            }

            DetailItem {
                id: canRemovePub
                label: "Can remove publication"
                value: contact.canRemovePublication
            }

            DetailItem {
                id: canBlock
                label: "Can block contacts"
                value: contact.blocked
            }

            DetailItem {
                id: canRequestSub
                label: "Can request subscription"
                value: contact.canRequestPresenceSubscription
            }

            DetailItem {
                id: canRescindSubRequest
                label: "Can rescind subscription request"
                value: contact.canRescindPresenceSubscriptionRequest
            }
        }

        // TODO: add contact avatar?


        menu: ContextMenu {
            MenuItem {
                text: qsTr("Authorise presence publication")
                visible: (contact.canAuthorisePublication && (contact.publishState === "ask"))
                onClicked: contact.authPubAction()
            }

            MenuItem {
                text: qsTr("Deny presence publication")
                visible: (contact.canRemovePublication && (contact.publishState === "ask"))
                onClicked: contact.denyPubAction()
            }

            MenuItem {
                text: qsTr("Remove presence pub & sub")
                visible: (contact.canRemovePublication && (contact.publishState === "yes"))
                onClicked: contact.removePubAction()
            }

            MenuItem {
                text: contact.blocked ? qsTr("Unblock") : qsTr("Block")
                visible: contact.canBlockContacts
                onClicked: contact.toggleBlockAction()
            }

            MenuItem {
                text: qsTr("Request presence subscription")
                visible: (contact.canRescindPresenceSubscriptionRequest && (contact.subscriptionState === "no"))
                onClicked: contact.requestSubAction()
            }

            MenuItem {
                text: qsTr("Rescind presence subscription request")
                visible: (contact.canRescindPresenceSubscriptionRequest && (contact.subscriptionState === "ask"))
                onClicked: contact.rescindSubRequestAction()
            }

        }

    }
}
