/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2017 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/qml/common/pages/About.qml
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

Page {
    id: sharingPage

    property string shareUrl
    property string shareTitle
    property var content

    Component.onCompleted: {
        if (!sharingPage.content) {
            sharingPage.content = new Object;
        }

        sharingPage.content.linkTitle = sharingPage.shareTitle
        sharingPage.content.status = sharingPage.shareUrl
        sharingPage.content.type = "text/x-url"
    }

    VerticalScrollDecorator { flickable: sharingList; page: sharingPage }

    SilicaListView {
        id: sharingList
        anchors.fill: parent
        contentHeight: Theme.itemSizeSmall
        model: SharingMethodsModel {}

        header: PageHeader {
            //: Page header for sharing a link
            //% "Share link"
            title: qsTrId("fuoten-share-link")
            page: sharingPage
            description: sharingPage.shareTitle
        }

        delegate: ListItem {
            id: sharingListItem
            contentWidth: parent.width
            contentHeight: Theme.itemSizeSmall

            Row {
                anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
                spacing: Theme.paddingSmall

                Label {
                    text: qsTrId(displayName)
                    color: sharingListItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

                Label {
                    text: userName
                    color: sharingListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                }
            }

            onClicked: {
                if (methodId !== "clipboard") {
                    pageStack.push(Qt.resolvedUrl(shareUiPath), {"methodId": methodId, "accountId": accountId, "content": sharingPage.content})
                } else {
                    Clipboard.text = sharingPage.shareUrl
                    pageStack.pop()
                }
            }
        }
    }
}
