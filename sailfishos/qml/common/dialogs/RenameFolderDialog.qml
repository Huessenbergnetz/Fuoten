/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.fuoten.items 1.0

Dialog {
    id: renameFolderDialog

    property string newName: ""

    property Folder folder: null

    canAccept: folder && (newName.length > 0) && (newName !== dHeader.title)

    onAccepted: if (folder) { folder.rename(renameFolderDialog.newName, config, localstorage) }

    SilicaFlickable {
        id: renameFolderDialogFlick
        anchors.fill: parent

        contentHeight: renameFolderCol.height + dHeader.height

        DialogHeader {
            id: dHeader
            acceptText: qsTrId("fuoten-rename-folder")
            dialog: renameFolderDialog
            flickable: renameFolderDialogFlick
            title: folder ? folder.name : ""
        }

        Column {
            id: renameFolderCol
            anchors { left: parent.left; right: parent.right; top: dHeader.bottom }
            spacing: Theme.paddingMedium

            TextField {
                id: newNameField
                width: parent.width
                label: qsTrId("fuoten-new-folder-name"); placeholderText: label
                text: folder ? folder.name : ""
                EnterKey.enabled: renameFolderDialog.canAccept
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: renameFolderDialog.accept()
                onTextChanged: renameFolderDialog.newName = text.trim()
            }
        }
    }
}
