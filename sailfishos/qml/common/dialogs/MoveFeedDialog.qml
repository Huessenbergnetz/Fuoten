/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/qml/phone/pages/MainPage.qml
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
import "../parts"

Dialog {
    id: moveFeedDialog

    property int newFolderId: 0

    property Feed feed: null

    canAccept: feed && (newFolderId !== feed.folderId)

    onAccepted: if (feed) { feed.move(moveFeedDialog.newFolderId, config, localstorage) }

    SilicaFlickable {
        id: moveFeedDialogFlick
        anchors.fill: parent

        contentHeight: moveFeedCol.height + dHeader.height

        DialogHeader {
            id: dHeader
            acceptText: qsTrId("fuoten-move-feed")
            dialog: moveFeedDialog
            flickable: moveFeedDialogFlick
            title: feed ? feed.title : ""
        }

        Column {
            id: moveFeedCol
            anchors { left: parent.left; right: parent.right; top: dHeader.bottom }
            spacing: Theme.paddingMedium

            Text {
                anchors {left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin }
                //: Text showing the current folder when moving a feed to a different folder
                //% "Current folder: %1"
                text: qsTrId("fuoten-current-folder-label").arg(feed.folderName
                                                                ? feed.folderName
                                                                : qsTrId("fuoten-no-folder")
                                                                )
                font.pixelSize: Theme.fontSizeMedium
                color: Theme.primaryColor
            }

            FoldersComboBox {
                //: Label for combo box to choose a folder a feed should be moved to
                //% "Target folder"
                label: qsTrId("fuoten-target-folder")
                onCurrentIndexChanged: if (currentItem) { newFolderId = currentItem.folderId }
            }
        }
    }
}
