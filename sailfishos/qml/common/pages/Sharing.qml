/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2019 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/qml/common/pages/Sharing.qml
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

            Item {
                anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin }
                height: parent.height

                Image {
                    id: shareIcon
                    source: iconUrl
                    width: Theme.iconSizeMedium
                    height: Theme.iconSizeMedium
                    anchors { left: parent.left; verticalCenter: parent.verticalCenter }
                }

                Label {
                    id: shareName
                    text: qsTrId(displayName)
                    color: sharingListItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                    anchors { left: shareIcon.right; leftMargin: Theme.paddingMedium; verticalCenter: parent.verticalCenter }
                }

                Label {
                    id: shareUser
                    text: userName
                    width: parent.width - shareIcon.width - shareName.width - 2 * parent.spacing
                    maximumLineCount: 1
                    truncationMode: TruncationMode.Fade
                    color: sharingListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    anchors { left: shareName.right; leftMargin: Theme.paddingSmall; right: parent.right; verticalCenter: parent.verticalCenter }
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
