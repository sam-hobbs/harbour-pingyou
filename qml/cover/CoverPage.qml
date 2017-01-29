/*

PingYou

Copyright (C) 2017 Sam Hobbs

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

CoverBackground {

    Image {
        id: coverIcon
        source: (accountsModel.numOnlineAccounts > 0) ? "qrc:///tux" : "qrc:///tux-sleeping"
        fillMode: Image.PreserveAspectFit
        cache: true
        //anchors.centerIn: parent
        width: parent.width
        x: Theme.paddingSmall
    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-s-message"
            onTriggered: console.log("cover action clicked")
        }
    }
}


