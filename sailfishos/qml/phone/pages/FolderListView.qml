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
    property string title: "Fuoten"

    ContextConfig {
        id: folderContextConfig
        contextType: startPage ? FuotenApp.StartPage : FuotenApp.Folders
    }

    Component.onCompleted: {
        if (!page.forwardNavigation && page.status === PageStatus.Active) {
            pageStack.pushAttached(Qt.resolvedUrl("../../common/pages/ContextConfigPage.qml"), {cc: folderContextConfig, name: startPage ? "" : title})
        }
    }

    Connections {
        target: page
        onStatusChanged: {
            if (page.status === PageStatus.Active && !page.forwardNavigation) {
                pageStack.pushAttached(Qt.resolvedUrl("../../common/pages/ContextConfigPage.qml"), {cc: folderContextConfig, name: startPage ? "" : title})
            }
        }
    }

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
        feedListDelegate: FeedListDelegate { folderView: true }
        onSearchTextChanged: folderListFlick.searchString = searchText
        onAllArticlesClicked: startPage ? pageStack.push(Qt.resolvedUrl("ArticlesListPage.qml"), {context: FuotenApp.AllItems}) : ""
        onStarredItemsClicked: pageStack.push(Qt.resolvedUrl("ArticlesListPage.qml"), {context: FuotenApp.StarredItems})
    }

    model: FolderListFilterModel {
        id: folderListModel
        sortingRole: folderContextConfig.sorting
        storage: localstorage
        search: folderListFlick.searchString
        hideRead: folderContextConfig.hideRead
        sortOrder: folderContextConfig.sortOrder
        Component.onCompleted: load()
    }

    delegate: ListItem {
        id: folderListItem

        contentHeight: Theme.itemSizeSmall

        menu: folderContextMenu

        ListView.onAdd: AddAnimation { target: folderListItem }
        ListView.onRemove: RemoveAnimation { target: folderListItem }

        onClicked: model.display.error ? model.display.clearError() : pageStack.push(Qt.resolvedUrl("FeedsListPage.qml"), {folder: model.display})

        ErrorItem {
            error: model.display.error
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
            highlighted: folderListItem.highlighted
            fontSize: Theme.fontSizeExtraSmall
        }

        RowLayout {
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
            spacing: Theme.paddingMedium
            visible: !model.display.error

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

                Text {
                    font.pixelSize: Theme.fontSizeTiny
                    color: folderListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    //% "%n feed(s)"
                    text: qsTrId("fuoten-feeds-count", model.display.feedCount)
                }
            }

            Label {
                text: model.display.unreadCount
                color: model.display.unreadCount ? Theme.highlightColor : folderListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                visible: !model.display.inOperation
                font.pixelSize: Theme.fontSizeMedium
            }

            BusyIndicator {
                size: BusyIndicatorSize.Small
                visible: model.display.inOperation
                running: model.display.inOperation
            }
        }

        RemorseItem { id: folderListItemRemorse }

        function deleteFolder() {
            //% "Deleting %1"
            folderListItemRemorse.execute(folderListItem, qsTrId("fuoten-deleting").arg(model.display.name), function() {model.display.remove(config, localstorage)})
        }

        function markFolderRead() {
            //% "Marking %1 read"
            folderListItemRemorse.execute(folderListItem, qsTrId("fuoten-marking-read").arg(model.display.name), function() {model.display.markAsRead(config, localstorage)})
        }

        Component {
            id: folderContextMenu
            ContextMenu {
                MenuItem {
                    //% "Rename folder"
                    text: qsTrId("fuoten-rename-folder")
                    enabled: !model.display.inOperation
                    onClicked: pageStack.push(Qt.resolvedUrl("../../common/dialogs/RenameFolderDialog.qml"), {folder: model.display})
                }
                MenuItem {
                    //% "Mark folder read"
                    text: qsTrId("fuoten-mark-folder-read")
                    enabled: !model.display.inOperation && model.display.unreadCount > 0
                    onClicked: folderListItem.markFolderRead()
                }

                MenuItem {
                    //% "Delete folder"
                    text: qsTrId("fuoten-delete-folder")
                    enabled: !model.display.inOperation
                    onClicked: folderListItem.deleteFolder()
                }
            }
        }
    }

    BusyIndicator {
        anchors.centerIn: parent
        size: BusyIndicatorSize.Large
        visible: folderListModel.inOperation
        running: folderListModel.inOperation
    }

    ViewPlaceholder {
        id: invalidAccountPlaceHolder
        flickable: folderListFlick
        enabled: folderListFlick.count === 0 && !config.isAccountValid && !folderListModel.inOperation
        //% "Invalid account configuration"
        text: qsTrId("fuoten-invalid-account")
        //% "Open the settings to configure your account"
        hintText: qsTrId("fuoten-invalid-account-hint")
    }

    ViewPlaceholder {
        id: emptyContent
        flickable: folderListFlick
        enabled: folderListFlick.count === 0 && config.isAccountValid && !folderListModel.inOperation
        //% "No folders found"
        text: qsTrId("fuoten-no-folders-found")
        //% "Synchronize your data or check your filter settings."
        hintText: qsTrId("fuoten-no-content-found-hint")
    }
}
