/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
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
