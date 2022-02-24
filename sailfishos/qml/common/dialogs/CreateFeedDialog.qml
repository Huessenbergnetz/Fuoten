/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.fuoten 1.0
import harbour.fuoten.api 1.0
import "../parts"

Page {
    id: createFeedDialog

    backNavigation: !createFeed.inOperation && !getItems.inOperation

    CreateFeed {
        id: createFeed
        configuration: config
        storage: localstorage
    }

    Connections {
        target: localstorage
        onCreatedFeed: {
            getItems.parentId = id
            getItems.execute()
        }
    }

    GetItems {
        id: getItems
        configuration: config
        storage: localstorage
        type: Fuoten.Feed
        onSucceeded: pageStack.pop()
    }

    SilicaFlickable {
        id: createFeedDialogFlick
        anchors.fill: parent

        contentHeight: createFeedCol.height

        Column {
            id: createFeedCol
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTrId("fuoten-add-feed")
                page: createFeedDialog
            }

            TextField {
                width: parent.width
                //: Label for a text input to set the URL of a new feed to add
                //% "Feed address"
                label: qsTrId("fuoten-feed-address"); placeholderText: label
                enabled: !createFeed.inOperation
                text: "http://"
                inputMethodHints: Qt.ImhUrlCharactersOnly
                EnterKey.enabled: createFeed.url.toString().length > 0
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: createFeed.execute()
                onTextChanged: createFeed.url = text
            }

            FoldersComboBox {
                onCurrentIndexChanged: if (currentItem) { createFeed.folderId = currentItem.folderId }
            }

            ErrorItem {
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                error: createFeed.error
            }

            BusyIndicator {
                size: BusyIndicatorSize.Large
                visible: createFeed.inOperation || getItems.inOperation
                running: createFeed.inOperation || getItems.inOperation
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
