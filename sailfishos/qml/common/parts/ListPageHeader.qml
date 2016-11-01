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
import harbour.fuoten.models 1.0

Column {
    property alias page: header.page
    property bool folders: true
    property bool startPage: true
    property bool feedListPage: false
    property alias searchVisible: searchField.visible
    property alias searchPlaceHolder: searchField.placeholderText
    property alias searchText: searchField.text
    property alias headerTitle: header.title
    property alias headerDescription: header.description

    width: parent ? parent.width : Screen.width


    PageHeader {
        id: header
        title: "Fuoten"
    }

    SearchField {
        id: searchField
        width: parent.width
        visible: false
        EnterKey.iconSource: "image://theme/icon-m-enter-close"
        EnterKey.onClicked: searchField.focus = false
        onVisibleChanged: {
            if (!visible) {
                text = ""
                searchField.focus = false
            } else {
                searchField.focus = true
            }
        }
    }

    ListItem {
        id: undreadItems
        contentHeight: Theme.itemSizeSmall
        visible: !searchField.visible && (startPage || feedListPage)

        RowLayout {
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
            spacing: Theme.paddingMedium

            Image {
                Layout.preferredWidth: folders ? Theme.iconSizeMedium : Theme.iconSizeSmall
                Layout.preferredHeight: folders ? Theme.iconSizeMedium : Theme.iconSizeSmall
                source: (folders ? "image://theme/icon-m-favorite-selected?" : "image://theme/icon-s-favorite?") + (undreadItems.highlighted ? (localstorage.totalUnread ? Theme.highlightColor : Theme.secondaryHighlightColor) : (undreadItems.totalUnread ? Theme.primaryColor : Theme.secondaryColor))
            }

            Label {
                Layout.fillWidth: true
                //% "Unread articles"
                text: qsTrId("fuoten-unread-articles")
                truncationMode: TruncationMode.Fade
                color: undreadItems.highlighted ? (localstorage.starred ? Theme.highlightColor : Theme.secondaryHighlightColor) : (localstorage.starred ? Theme.primaryColor : Theme.secondaryColor)
            }

            Label {
                text: localstorage.totalUnread
                color: localstorage.totalUnread ? Theme.highlightColor : undreadItems.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                font.pixelSize: Theme.fontSizeMedium
            }
        }
    }

    ListItem {
        id: starredItems
        contentHeight: Theme.itemSizeSmall
        visible: !searchField.visible && startPage

        RowLayout {
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
            spacing: Theme.paddingMedium

            Image {
                Layout.preferredWidth: folders ? Theme.iconSizeMedium : Theme.iconSizeSmall
                Layout.preferredHeight: folders ? Theme.iconSizeMedium : Theme.iconSizeSmall
                source: (folders ? "image://theme/icon-m-favorite-selected?" : "image://theme/icon-s-favorite?") + (starredItems.highlighted ? (localstorage.starred ? Theme.highlightColor : Theme.secondaryHighlightColor) : (localstorage.starred ? Theme.primaryColor : Theme.secondaryColor))
            }

            Label {
                Layout.fillWidth: true
                //% "Starred articles"
                text: qsTrId("fuoten-starred-articles")
                truncationMode: TruncationMode.Fade
                color: starredItems.highlighted ? (localstorage.starred ? Theme.highlightColor : Theme.secondaryHighlightColor) : (localstorage.starred ? Theme.primaryColor : Theme.secondaryColor)
            }

            Label {
                text: localstorage.starred
                color: localstorage.starred ? Theme.highlightColor : starredItems.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                font.pixelSize: Theme.fontSizeMedium
            }
        }
    }

    Repeater {
        model: FeedListFilterModel {
            doubleParentId: 0
            storage: folders ? localstorage : null
            Component.onCompleted: if (folders && startPage) { load() }
        }

        delegate: ListItem {
            id: feedListItem

            contentHeight: Theme.itemSizeSmall

            menu: feedContextMenu

            ListView.onAdd: AddAnimation { target: feedListItem }
            ListView.onRemove: RemoveAnimation { target: feedListItem }

            RowLayout {
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
                spacing: Theme.paddingMedium
                visible: !display.error

                Item {
                    Layout.preferredHeight: Theme.iconSizeMedium
                    Layout.preferredWidth: Theme.iconSizeMedium

                    CachedImage {
                        sourceUrl: display.faviconLink
                        sourceSize.width: Theme.iconSizeMedium - (20 * Theme.pixelRatio)
                        sourceSize.height: Theme.iconSizeMedium - (20 * Theme.pixelRatio)
                        anchors.centerIn: parent
                    }
                }

                Label {
                    Layout.fillWidth: true
                    font.pixelSize: Theme.fontSizeMedium
                    text: display.title
                    truncationMode: TruncationMode.Fade
                    color: feedListItem.highlighted ? (display.unreadCount ? Theme.highlightColor : Theme.secondaryHighlightColor) : (display.unreadCount ? Theme.primaryColor : Theme.secondaryColor)
                    textFormat: Text.StyledText
                }

                Label {
                    text: display.unreadCount
                    color: display.unreadCount ? Theme.highlightColor : feedListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    visible: !display.inOperation
                    font.pixelSize: Theme.fontSizeMedium
                }

                BusyIndicator {
                    size: BusyIndicatorSize.Small
                    visible: display.inOperation
                    running: display.inOperation
                }
            }

            RemorseItem { id: feedListItemRemorse }

            function deleteFeed() {
                //% "Deleting %1"
                feedListItemRemorse.execute(feedListItem, qsTrId("fuoten-deleting").arg(display.title), function() {display.remove(config, localstorage)})
            }

            Component {
                id: feedContextMenu
                ContextMenu {
                    MenuItem {
                        //% "Rename feed"
                        text: qsTrId("fuoten-rename-feed")
                        enabled: !model.display.inOperation
                        onClicked: pageStack.push(Qt.resolvedUrl("../dialogs/RenameFeedDialog.qml"), {feed: display})
                    }
                    MenuItem {
                        //% "Delete feed"
                        text: qsTrId("fuoten-delete-feed")
                        enabled: !display.inOperation
                        onClicked: feedListItem.deleteFeed()
                    }
                }
            }
        }
    }
}

