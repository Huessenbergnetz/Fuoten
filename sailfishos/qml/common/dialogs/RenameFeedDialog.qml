/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2018 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/qml/common/dialogs/RenameFeedDialog.qml
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.fuoten.items 1.0

Dialog {
    id: renameFeedDialog

    property string newName: ""

    property Feed feed: null

    canAccept: feed && (newName.length > 0) && (newName !== dHeader.title)

    onAccepted: if (feed) { feed.rename(renameFeedDialog.newName, config, localstorage) }

    SilicaFlickable {
        id: renameFeedDialogFlick
        anchors.fill: parent

        contentHeight: renameFeedCol.height + dHeader.height

        DialogHeader {
            id: dHeader
            acceptText: qsTrId("fuoten-rename-feed")
            dialog: renameFeedDialog
            flickable: renameFeedDialogFlick
            title: feed ? feed.title : ""
        }

        Column {
            id: renameFeedCol
            anchors { left: parent.left; right: parent.right; top: dHeader.bottom }
            spacing: Theme.paddingMedium

            TextField {
                id: newNameField
                width: parent.width
                //: Label and placeholder text for a text input when renaming a feed
                //% "New feed name"
                label: qsTrId("fuoten-new-feed-name"); placeholderText: label
                text: feed ? feed.title : ""
                EnterKey.enabled: renameFeedDialog.canAccept
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                onTextChanged: renameFeedDialog.newName = text.trim()
            }
        }
    }
}
