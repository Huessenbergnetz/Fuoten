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
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0

Column {
    property alias page: header.page
    property bool folders: true
    property bool startPage: true
    property alias searchVisible: searchField.visible
    property alias searchPlaceHolder: searchField.placeholderText
    property alias searchText: searchField.text
    property alias headerTitle: header.title

    width: parent.width


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
        contentHeight: folders ? Theme.itemSizeMedium : Theme.itemSizeSmall
        visible: !searchField.visible && startPage

        RowLayout {
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
            spacing: Theme.paddingMedium

            Image {
                Layout.preferredWidth: folders ? Theme.iconSizeMedium : Theme.iconSizeSmall
                Layout.preferredHeight: folders ? Theme.iconSizeMedium : Theme.iconSizeSmall
                source: (folders ? "image://theme/icon-m-favorite-selected?" : "image://theme/icon-s-favorite?") + (undreadItems.highlighted ? (localstorage.starred ? Theme.highlightColor : Theme.secondaryHighlightColor) : (localstorage.starred ? Theme.primaryColor : Theme.secondaryColor))
            }

            ColumnLayout {
                spacing: Theme.paddingSmall

                Label {
                    Layout.fillWidth: true
                    //% "Undread articles"
                    text: qsTrId("fuoten-unread-articles")
                    truncationMode: TruncationMode.Fade
                    color: undreadItems.highlighted ? (localstorage.starred ? Theme.highlightColor : Theme.secondaryHighlightColor) : (localstorage.starred ? Theme.primaryColor : Theme.secondaryColor)
                }

                Text {
                    Layout.fillWidth: true
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: undreadItems.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    //% "%n Unread"
                    text: qsTrId("fuoten-unread-count", localstorage.starred)
                    visible: folders
                }
            }
        }
    }

    ListItem {
        id: starredItems
        contentHeight: folders ? Theme.itemSizeMedium : Theme.itemSizeSmall
        visible: !searchField.visible && startPage

        RowLayout {
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
            spacing: Theme.paddingMedium

            Image {
                Layout.preferredWidth: folders ? Theme.iconSizeMedium : Theme.iconSizeSmall
                Layout.preferredHeight: folders ? Theme.iconSizeMedium : Theme.iconSizeSmall
                source: (folders ? "image://theme/icon-m-favorite-selected?" : "image://theme/icon-s-favorite?") + (starredItems.highlighted ? (localstorage.starred ? Theme.highlightColor : Theme.secondaryHighlightColor) : (localstorage.starred ? Theme.primaryColor : Theme.secondaryColor))
            }

            ColumnLayout {
                spacing: Theme.paddingSmall

                Label {
                    Layout.fillWidth: true
                    //% "Starred articles"
                    text: qsTrId("fuoten-starred-articles")
                    truncationMode: TruncationMode.Fade
                    color: starredItems.highlighted ? (localstorage.starred ? Theme.highlightColor : Theme.secondaryHighlightColor) : (localstorage.starred ? Theme.primaryColor : Theme.secondaryColor)
                }

                Text {
                    Layout.fillWidth: true
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: starredItems.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    //% "%n Starred"
                    text: qsTrId("fuoten-starred-count", localstorage.starred)
                    visible: folders
                }
            }
        }
    }
}

