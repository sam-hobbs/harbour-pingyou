import QtQuick 2.2
import Sailfish.Silica 1.0

import "../delegates"

Page {
    id: page
    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView
        model: accountsModel
        anchors.fill: parent
        width: parent.width
        header: PageHeader {
            title: "Accounts"
        }
        delegate: AccountEntry { }
        VerticalScrollDecorator {}

    }

}
