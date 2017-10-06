/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
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
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
import harbour.fuoten 1.0
import harbour.fuoten.items 1.0
import "../../common/parts"

ListItem {
        id: feedListItem

        property string searchString
        property bool folderView: false
        property Page page

        contentHeight: Theme.itemSizeSmall

        menu: feedContextMenu

        ListView.onAdd: AddAnimation { target: feedListItem }
        ListView.onRemove: RemoveAnimation { target: feedListItem }

        onClicked: model.display.error ? model.display.clearError() : pageStack.push(Qt.resolvedUrl("ArticlesListPage.qml"), {context: FuotenApp.FeedItems, feed: model.display})

        ErrorItem {
            error: model.display.error
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
            highlighted: feedListItem.highlighted
            fontSize: Theme.fontSizeExtraSmall
        }

        RowLayout {
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
            spacing: Theme.paddingMedium
            visible: !model.display.error

            Item {
                Layout.preferredHeight: !folderView ? Theme.iconSizeSmall : Theme.iconSizeMedium
                Layout.preferredWidth: !folderView ? Theme.iconSizeSmall : Theme.iconSizeMedium

                Image {
                    source: display.faviconLink.toString() !== "" ? display.faviconLink : (!folderView ? "image://fuoten/icon-s-feed?" : "image://fuoten/icon-m-feed?") + (feedListItem.highlighted ? Theme.highlightColor : Theme.primaryColor)
                    sourceSize.width: !folderView ? Theme.iconSizeSmall : Theme.iconSizeMedium - (20 * Theme.pixelRatio)
                    sourceSize.height: !folderView ? Theme.iconSizeSmall : Theme.iconSizeMedium - (20 * Theme.pixelRatio)
                    anchors.centerIn: !folderView ? undefined : parent
                    anchors.fill: !folderView ? parent : undefined
                    onStatusChanged: {
                        if (status === Image.Error) {
                            source = (!folderView ? "image://fuoten/icon-s-feed?" : "image://fuoten/icon-m-feed?") + (feedListItem.highlighted ? Theme.highlightColor : Theme.primaryColor)
                        }
                    }
                }
            }

            Label {
                Layout.fillWidth: true
                font.pixelSize: Theme.fontSizeMedium
                text: Theme.highlightText(model.display ? model.display.title : "", feedListItem.searchString, Theme.highlightColor)
                truncationMode: TruncationMode.Fade
                color: feedListItem.highlighted ? (model.display.unreadCount ? Theme.highlightColor : Theme.secondaryHighlightColor) : (model.display.unreadCount ? Theme.primaryColor : Theme.secondaryColor)
                textFormat: Text.StyledText
            }

            Label {
                text: model.display.unreadCount
                color: model.display.unreadCount ? Theme.highlightColor : feedListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                visible: model.display && !model.display.inOperation
                font.pixelSize: Theme.fontSizeMedium
            }

            BusyIndicator {
                size: BusyIndicatorSize.Small
                visible: model.display && model.display.inOperation
                running: model.display && model.display.inOperation
            }
        }

//        RemorseItem { id: feedListItemRemorse }

        function deleteFeed() {
            if (feedListItem.folderView) {
                //% "Deleting %1"
                Remorse.popupAction(feedListItem.page, qsTrId("fuoten-deleting").arg(model.display.title), function() {model.display.remove(config, localstorage)})
            } else {
                Remorse.itemAction(feedListItem, qsTrId("fuoten-deleting"), function() {model.display.remove(config, localstorage)})
            }

//            feedListItemRemorse.execute(feedListItem, qsTrId("fuoten-deleting").arg(model.display.title), function() {model.display.remove(config, localstorage)})
        }

        Component {
            id: feedContextMenu
            ContextMenu {
                MenuItem {
                    //% "Rename feed"
                    text: qsTrId("fuoten-rename-feed")
                    enabled: !model.display.inOperation
                    onClicked: pageStack.push(Qt.resolvedUrl("../../common/dialogs/RenameFeedDialog.qml"), {feed: model.display})
                }
                MenuItem {
                    //% "Mark feed read"
                    text: qsTrId("fuoten-mark-feed-read")
                    enabled: !model.display.inOpeartion
                    onClicked: model.display.markAsRead(config, localstorage, true)
                }
                MenuItem {
                    //% "Move feed"
                    text: qsTrId("fuoten-move-feed")
                    enabled: !model.display.inOperation
                    onClicked: pageStack.push(Qt.resolvedUrl("../../common/dialogs/MoveFeedDialog.qml"), {feed: model.display})
                }
                MenuItem {
                    //% "Delete feed"
                    text: qsTrId("fuoten-delete-feed")
                    enabled: !model.display.inOperation
                    onClicked: feedListItem.deleteFeed()
                }
            }
        }
    }
