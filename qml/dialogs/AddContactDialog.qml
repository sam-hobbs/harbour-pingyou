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

Dialog {

    canAccept: jabberIDField.acceptableInput

    Column {
        width: parent.width

        DialogHeader {
            id: header
            title: "Add contact"
            acceptText: "Add"
        }

        TextField {
            id: jabberIDField
            width: parent.width
            placeholderText: "romeo@montague.net"
            label: "Jabber ID"
            focus: true
            validator: RegExpValidator { regExp: /^\w+@\w+(\.\w+)+$/ }
        }
    }

    onDone: {
        if (result == DialogResult.Accepted) {
            rosterModel.addContact(jabberIDField.text)
        }
    }
}