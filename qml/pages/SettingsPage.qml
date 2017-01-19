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

Page {
    objectName: "SettingsPage"

    PageHeader {
        id: header
        title: qsTr("Settings")
    }

    SilicaFlickable {
        id: flickable

        anchors {
            top: header.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        contentHeight: content.height

        Column {
            id: content
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.paddingMedium
            anchors.rightMargin: Theme.paddingMedium

            TextSwitch {
                text: "Show Developer Information"
                description: "Exposes additonal information that may be useful for developers"
                checked: settings.displayDevInfo
                onCheckedChanged: {
                    settings.displayDevInfo = checked
                }

            }

        }

        VerticalScrollDecorator {flickable: flickable}
    }
}
