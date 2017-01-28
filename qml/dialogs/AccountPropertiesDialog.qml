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

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.pingyou 0.1

// https://stackoverflow.com/questions/17017365/use-a-qlistqvariantmap-as-a-model-for-a-qml-listview#17146819
// https://doc.qt.io/qt-5/qtqml-cppintegration-data.html#qvariantlist-and-qvariantmap-to-javascript-array-and-object

Dialog {
    id: accountPropertiesDialog

    property variant parameterModel // set when dialog is opened
    property bool existingAccount // set when dialog is opened

//    AccountPropertiesDialogData {
//        id: parameterModel
//    }

//    property AccountPropertiesDialogData parameterModel;

    SilicaFlickable {
        anchors.fill: parent
        height: content.height
        contentHeight: content.height
        width: parent.width

        Column {
            id: content

            width: parent.width

            DialogHeader {
                title: "Connection Settings"
                width: parent.width
            }

            SectionHeader  { text: "Basic settings" }

            TextField {
                id: accountItem
                //text: parameterModel.account
                text: parameterModel["account"] ? parameterModel["account"] : ""
                inputMethodHints: Qt.ImhEmailCharactersOnly
                placeholderText: "Jabber ID (romeo@montague.net)"

                label: "Account"
                labelVisible: true

                width: parent.width

                focus: true
            }

            TextField {
                id: passwordItem
                //text: parameterModel.password
                text: parameterModel["password"] ? parameterModel["password"] : ""
                inputMethodHints: Qt.ImhHiddenText
                placeholderText: "Password"
                echoMode: TextInput.Password

                label: "Password"
                labelVisible: true

                width: parent.width
            }

            TextSwitch {
                id: registerItem
                visible: existingAccount ? false : true
                text: "Register"
                description: "Register this account with the server"
                //checked: parameterModel.register
                checked: parameterModel["register"] ? parameterModel["register"] : false
            }

            SectionHeader { text: "Server settings" }

            TextField {
                id: serverItem
                //text: parameterModel.server
                text: parameterModel["server"] ? parameterModel["server"] : ""
                inputMethodHints: Qt.ImhNoAutoUppercase
                placeholderText: "Server address"

                label: "Server address"
                labelVisible: true

                width: parent.width
            }

            TextField {
                id: portItem
                //text: parameterModel.port
                text: parameterModel["port"] ? parameterModel["port"] : ""
                inputMethodHints: Qt.ImhDigitsOnly
                placeholderText: "5222"

                label: "Port"
                labelVisible: true

                width: parent.width
            }

            TextSwitch {
                id: oldSSLItem
                text: "Old SSL"
                description: "Connect using SSL instead of STARTTLS"
                //checked: parameterModel.oldSSL
                checked: parameterModel["old-ssl"] ? parameterModel["old-ssl"] : false
                width: parent.width
            }

            TextField {
                id: fallbackServersItem
                //text: "" // invalid
                //text: parameterModel.fallbackServers
                text: parameterModel["fallback-servers"] ? parameterModel["fallback-servers"] : ""
                inputMethodHints: Qt.ImhUrlCharactersOnly
                placeholderText: "Fallback servers"

                label: "Fallback servers"
                labelVisible: true

                width: parent.width
            }

            TextField {
                id: fallbackConferenceServerItem
                //text: ""
                //text: parameterModel.fallbackConferenceServer
                text: parameterModel["fallback-conference-server"] ? parameterModel["fallback-conference-server"] : ""
                placeholderText: "Fallback conference server"
                inputMethodHints: Qt.ImhUrlCharactersOnly

                label: "Fallback conference server"
                labelVisible: true

                width: parent.width
            }

            TextField {
                id: fallbackSocks5ProxiesItem
                //text: "" // qstringlist (empty)
                //text: parameterModel.fallbackSocks5Proxies
                text: parameterModel["fallback-socks5-proxies"] ? parameterModel["fallback-socks5-proxies"] : ""
                placeholderText: "Fallback SOCKS5 proxies"

                label: "Fallback SOCKS5 proxies"
                labelVisible: true

                width: parent.width
            }

            SectionHeader { text: "Security settings" }

            IconTextSwitch {
                id: requireEncryptionItem
                text: "Require encryption"
                icon.source: checked ? "image://theme/icon-m-device-lock" : "image://theme/icon-m-dismiss"
                description: "Mandatory tranport encryption"
                //checked: true
                //checked: parameterModel.requireEncryption
                checked: parameterModel["require-encryption"] ? parameterModel["require-encryption"] : true
                width: parent.width
            }

            // properties with hyphens in the name https://stackoverflow.com/questions/7122609/how-do-i-reference-a-javascript-object-property-with-a-hyphen-in-it
            // icon names https://sailfishos.org/develop/docs/jolla-ambient/
            IconTextSwitch {
                id: ignoreSSLItem
                text: "Ignore SSL Errors?"
                icon.source: checked ? "image://theme/icon-m-dismiss" : "image://theme/icon-m-device-lock"
                description: "Allows connection to servers without valid TLS certificates"
                checked: parameterModel["ignore-ssl-errors"] ? parameterModel["ignore-ssl-errors"] : false
                width: parent.width
            }

            TextField {
                id: extraCertificateIdentitiesItem
                //text: "" // invalid
                //text: parameterModel.extraCertificateIdentities
                text: parameterModel["extra-certificate-identities"] ? parameterModel["extra-certificate-identities"] : ""
                placeholderText: "Extra certificate identities"

                label: "Extra certificate identities"
                labelVisible: true

                width: parent.width
            }

            SectionHeader { text: "Resource settings" }


            TextField {
                id: resourceNameItem
                //text: parameterModel.resource
                text: parameterModel["resource"] ? parameterModel["resource"] : "PingYou"
                placeholderText: "PingYou"
                inputMethodHints: Qt.ImhNoAutoUppercase

                label: "Resource name"
                labelVisible: true

                width: parent.width
            }

            TextField {
                id: aliasItem
                //text: ""
                //text: parameterModel.alias
                text: parameterModel["alias"] ? parameterModel["alias"] : ""
                placeholderText: "Alias"
                inputMethodHints: Qt.ImhNoPredictiveText

                label: "Alias"
                labelVisible: true

                width: parent.width
            }

            Slider {
                id: priorityItem
                width: parent.width
                minimumValue: -256
                maximumValue: 256
                //value: 0
                //value: parameterModel.priority
                value: parameterModel["priority"] ? parameterModel["priority"] : 0
                valueText: value
                label: "Priority"
                stepSize: 1
            }

            SectionHeader { text: "Network settings" }

            TextField {
                id: keepaliveIntervalItem
                //text: ""
                //text: parameterModel.keepaliveInterval
                text: parameterModel["keepalive-interval"] ? parameterModel["keepalive-interval"] : ""
                placeholderText: "Keepalive Interval"
                inputMethodHints: Qt.ImhDigitsOnly

                label: "Keepalive interval"
                labelVisible: true

                width: parent.width
            }

            TextSwitch {
                id: lowBandwidthItem
                text: "Low bandwidth"
                //description: "Not sure what this does"
                //checked: false
                //checked: parameterModel.lowBandwidth
                checked: parameterModel["low-bandwidth"] ? parameterModel["low-bandwidth"] : false
            }


            SectionHeader { text: "HTTP proxy settings" }

            TextField {
                id: httpsProxyServerItem
                //text: ""
                //text: parameterModel.httpsProxyServer
                text: parameterModel["https-proxy-server"] ? parameterModel["https-proxy-server"] : ""
                placeholderText: "HTTPS proxy server"
                inputMethodHints: Qt.ImhUrlCharactersOnly

                label: "HTTPS proxy server"
                labelVisible: true

                width: parent.width
            }

            TextField {
                id: httpsProxyPortItem
                //text: "443"
                //text: parameterModel.httpsProxyPort
                text: parameterModel["https-proxy-port"] ? parameterModel["https-proxy-port"] : ""
                placeholderText: "HTTPS proxy port"
                inputMethodHints: Qt.ImhDigitsOnly

                label: "HTTPS proxy port"
                labelVisible: true

                width: parent.width
            }

            SectionHeader { text: "STUN settings" }

            TextField {
                id: stunServerItem
                //text: ""
                //text: parameterModel.stunServer
                text: parameterModel["stun-server"] ? parameterModel["stun-server"] : ""
                placeholderText: "STUN server"
                inputMethodHints: Qt.ImhUrlCharactersOnly

                label: "STUN server"
                labelVisible: true

                width: parent.width
            }

            TextField {
                id: stunPortItem
                //text: ""
                //text: parameterModel.stunPort
                text: parameterModel["stun-port"] ? parameterModel["stun-port"] : ""
                placeholderText: "STUN port"
                inputMethodHints: Qt.ImhDigitsOnly

                label: "STUN port"
                labelVisible: true

                width: parent.width
            }

            TextField {
                id: fallbackStunServerItem
                //text: ""
                //text: parameterModel.fallbackStunServer
                text: parameterModel["fallback-stun-server"] ? parameterModel["fallback-stun-server"] : ""
                placeholderText: "Fallback STUN server"
                inputMethodHints: Qt.ImhUrlCharactersOnly

                label: "Fallback STUN server"
                labelVisible: true

                width: parent.width
            }

            TextField {
                id: fallbackStunPortItem
                // text: ""
                //text: parameterModel.fallbackStunPort
                text: parameterModel["fallback-stun-port"] ? parameterModel["fallback-stun-port"] : ""
                placeholderText: "Fallback STUN port"
                inputMethodHints: Qt.ImhDigitsOnly

                label: "Fallback STUN port"
                labelVisible: true

                width: parent.width
            }

            SectionHeader { text: "Misc" }

            TextSwitch {
                id: decloakAutomaticallyItem
                // org.freedesktop.Telepathy.Connection.Interface.Gabble.Decloak.DecloakAutomatically
                text: "Decloak automatically"
                //description: "Decloak automatically"
                //checked: true
                //checked: parameterModel.decloakAutomatically
                checked: parameterModel["decloak-automatically"] ? parameterModel["decloak-automatically"] : ""
                width: parent.width
            }

            TextSwitch {
                id: downloadAtConnectionItem
                // org.freedesktop.Telepathy.Connection.Interface.ContactList.DownloadAtConnection
                text: "Download contact list at connection"
                description: "Retrieves the roster during connection"
                //checked: true
                //checked: parameterModel.downloadAtConnection
                checked: parameterModel["download-at-connection"] ? parameterModel["download-at-connection"] : ""
            }


//            Button {
//                id: button
//                text: "read values"
//                width: parent.width
//                onClicked: content.readValues(parameterModel)
//            }


//            function readValues(parameterModel) {
//                console.log("readValues called, number of properties in parameterModel is: " + parameterModel.length)
//                for (var prop in parameterModel) {
//                    console.log("Object item:", prop, "=", parameterModel[prop])
//                }
//            }

        }
    }

    onDone: {
        if (result == DialogResult.Accepted) {
            // if this is an existing account, call method to update properties. Otherwise, create a new account
            if (existingAccount) {
                console.log("Updating account parameters")

                // https://supportforums.blackberry.com/t5/Native-Development/Passing-QVariantMap-from-QML/td-p/2473237
                //accountsModel.createAccount({"ignore-ssl-errors": ignoreSSLItem.checked,"fallback-stun-port": fallbackStunPortItem.text})
                accountsModel.createAccount({"account": accountItem.text, "password": passwordItem.text, "server": serverItem.text, "port": portItem.text, "old-ssl": oldSSLItem.checked, "fallback-servers": fallbackServersItem.text, "fallback-conference-server": fallbackConferenceServerItem.text, "fallback-socks5-proxies": fallbackSocks5ProxiesItem.text, "require-encryption": requireEncryptionItem.checked, "ignore-ssl-errors": ignoreSSLItem.checked, "extra-certificate-identities": extraCertificateIdentitiesItem.text, "resource": resourceNameItem.text, "alias": aliasItem.text, "priority": priorityItem.value, "keepalive-interval": keepaliveIntervalItem.text, "low-bandwidth": lowBandwidthItem.checked, "https-proxy-server": httpsProxyServerItem.text, "https-proxy-port": httpsProxyPortItem.text, "stun-server": stunServerItem.text, "stun-port": stunPortItem.text, "fallback-stun-server": fallbackStunServerItem.text, "fallback-stun-port": fallbackStunPortItem.text, "org.freedesktop.Telepathy.Connection.Interface.Gabble.Decloak.DecloakAutomatically": decloakAutomaticallyItem.checked, "org.freedesktop.Telepathy.Connection.Interface.ContactList.DownloadAtConnection": downloadAtConnectionItem.checked})

            } else {
                console.log("Creating new account using supplied parameters")
                //accountsModel.createAccount(parameterModel)
                //accountsModel.createAccount({"ignore-ssl-errors": ignoreSSLItem.checked,"fallback-stun-port": fallbackStunPortItem.text})
                accountsModel.createAccount({"account": accountItem.text, "password": passwordItem.text, "register": registerItem.checked, "server": serverItem.text, "port": portItem.text, "old-ssl": oldSSLItem.checked, "fallback-servers": fallbackServersItem.text, "fallback-conference-server": fallbackConferenceServerItem.text, "fallback-socks5-proxies": fallbackSocks5ProxiesItem.text, "require-encryption": requireEncryptionItem.checked, "ignore-ssl-errors": ignoreSSLItem.checked, "extra-certificate-identities": extraCertificateIdentitiesItem.text, "resource": resourceNameItem.text, "alias": aliasItem.text, "priority": priorityItem.value, "keepalive-interval": keepaliveIntervalItem.text, "low-bandwidth": lowBandwidthItem.checked, "https-proxy-server": httpsProxyServerItem.text, "https-proxy-port": httpsProxyPortItem.text, "stun-server": stunServerItem.text, "stun-port": stunPortItem.text, "fallback-stun-server": fallbackStunServerItem.text, "fallback-stun-port": fallbackStunPortItem.text, "org.freedesktop.Telepathy.Connection.Interface.Gabble.Decloak.DecloakAutomatically": decloakAutomaticallyItem.checked, "org.freedesktop.Telepathy.Connection.Interface.ContactList.DownloadAtConnection": downloadAtConnectionItem.checked})
            }
        }
    }
}
