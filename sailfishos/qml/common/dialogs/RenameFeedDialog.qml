/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
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
                EnterKey.onClicked: renameFeedDialog.accept()
                onTextChanged: renameFeedDialog.newName = text.trim()
            }
        }
    }
}
