/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.fuoten.api 1.0
import "../parts"

Page {
    id: createFolderDialog

    backNavigation: !createFolder.inOperation

    CreateFolder {
        id: createFolder
        configuration: config
        storage: localstorage
        onSucceeded: {
            pageStack.pop()
        }
    }

    SilicaFlickable {
        id: createFolderDialogFlick
        anchors.fill: parent

        contentHeight: createFolderCol.height

        Column {
            id: createFolderCol
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTrId("fuoten-create-folder")
                page: createFolderDialog
            }

            TextField {
                id: folderNameField
                width: parent.width
                //: Label and placeholder text for a text input to set the name for a new folder or to rename an existing folder
                //% "New folder name"
                label: qsTrId("fuoten-new-folder-name"); placeholderText: label
                enabled: !createFolder.inOperation
                EnterKey.enabled: createFolder.name.length > 0
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: createFolder.execute()
                onTextChanged: createFolder.name = text
            }

            ErrorItem {
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                error: createFolder.error
            }

            BusyIndicator {
                size: BusyIndicatorSize.Large
                visible: createFolder.inOperation
                running: createFolder.inOperation
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
