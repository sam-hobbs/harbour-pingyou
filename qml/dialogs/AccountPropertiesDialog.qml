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

// https://stackoverflow.com/questions/17017365/use-a-qlistqvariantmap-as-a-model-for-a-qml-listview#17146819

Dialog {

    //canAccept: jabberIDField.acceptableInput

    property variant parameterModel // set when dialog is opened

    SilicaListView {

        DialogHeader {}

        id: list_view

        anchors.fill: parent
        model: parameterModel
        // modelData holds the data with the same role name
        // model is the model
        // index is the index of the current item
        delegate:
                Label {
                    text: model.account
                }
    }



    onDone: {
        if (result == DialogResult.Accepted) {
            //rosterModel.addContact(jabberIDField.text)
            console.log("accepted")
        }
    }
}
