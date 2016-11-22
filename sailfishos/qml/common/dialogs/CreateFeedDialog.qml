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
                //% "Add feed"
                title: qsTrId("fuoten-add-feed")
                page: createFeedDialog
            }

            TextField {
                width: parent.width
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
                visible: createFeed.inOperation
                running: createFeed.inOperation
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
