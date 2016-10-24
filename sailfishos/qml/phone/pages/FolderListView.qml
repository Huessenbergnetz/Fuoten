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
import "../../common/parts"


SilicaListView {
    id: folderListFlick
    anchors.fill: parent
    currentIndex: -1

    property string searchString
    property Item page
    property bool searchVisible: false
    property bool startPage: true
    property alias sortingRole: folderListModel.sortingRole
    property alias sortOrder: folderListModel.sortOrder
    property alias hideRead: folderListModel.hideRead

    PullDownMenu {
        busy: synchronizer.inOperation

        MenuItem {
            //% "About"
            text: qsTrId("id-about")
            onClicked: pageStack.push(Qt.resolvedUrl("../../common/pages/About.qml"))
            visible: startPage
        }

        MenuItem {
            //% "Settings"
            text: qsTrId("id-settings")
            onClicked: pageStack.push(Qt.resolvedUrl("../../common/pages/Settings.qml"))
            visible: startPage
        }

        MenuItem {
            text: folderListFlick.searchVisible
                    //% "Hide search"
                  ? qsTrId("fuoten-hide-search")
                    //% "Show search"
                  : qsTrId("fuoten-show-search")
            onClicked: folderListFlick.searchVisible = !folderListFlick.searchVisible
        }

        MenuItem {
            //% "Synchronize"
            text: qsTrId("fuoten-synchronize")
            onClicked: synchronizer.sync()
            enabled: !synchronizer.inOperation
        }
    }

    VerticalScrollDecorator { flickable: folderListFlick; page: folderListFlick.page }

    header: ListPageHeader {
        id: folderListHeader
        page: folderListFlick.page
        searchVisible: folderListFlick.searchVisible
        startPage: folderListFlick.startPage
        onSearchTextChanged: folderListFlick.searchString = searchText
    }

    model: FolderListFilterModel {
        id: folderListModel
        sortingRole: config.mainViewSorting
        storage: localstorage
        search: folderListFlick.searchString
        hideRead: config.mainViewHideRead
        sortOrder: config.mainViewSortOrder
        Component.onCompleted: load()
    }

    delegate: ListItem {
        id: folderListItem

        contentHeight: Theme.itemSizeSmall

        ListView.onAdd: AddAnimation { target: folderListItem }
        ListView.onRemove: RemoveAnimation { target: folderListItem }

        RowLayout {
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
            spacing: Theme.paddingMedium

            Image {
                Layout.preferredWidth: Theme.iconSizeMedium
                Layout.preferredHeight: Theme.iconSizeMedium
                source: "image://theme/icon-m-folder?" + (folderListItem.highlighted ? (model.display.unreadCount ? Theme.highlightColor : Theme.secondaryHighlightColor) : (model.display.unreadCount ? Theme.primaryColor : Theme.secondaryColor))
            }

            ColumnLayout {
                spacing: Theme.paddingSmall/2

                Label {
                    Layout.fillWidth: true
                    font.pixelSize: Theme.fontSizeSmall
                    text: Theme.highlightText(model.display ? model.display.name : "", folderListFlick.searchString, Theme.highlightColor)
                    truncationMode: TruncationMode.Fade
                    color: folderListItem.highlighted ? (model.display.unreadCount ? Theme.highlightColor : Theme.secondaryHighlightColor) : (model.display.unreadCount ? Theme.primaryColor : Theme.secondaryColor)
                    textFormat: Text.StyledText
                }

                RowLayout {
                    spacing: Theme.paddingSmall/2

                    Text {
                        font.pixelSize: Theme.fontSizeTiny
                        color: folderListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        //% "%n Feed(s)"
                        text: qsTrId("fuoten-feeds-count", model.display.feedCount)
                    }

                    Text {
                        font.pixelSize: Theme.fontSizeTiny
                        color: folderListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        text: "|"
                    }

                    Text {
                        font.pixelSize: Theme.fontSizeTiny
                        color: folderListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        //% "%n Item(s)"
                        text: qsTrId("fuoten-items-count", model.display.itemCount)
                        Layout.fillWidth: true
                    }
                }
            }

            CountBubble {
                value: model.display.unreadCount
                color: folderListItem.highlighted ? Theme.highlightColor : Theme.primaryColor
            }
        }
    }
}
