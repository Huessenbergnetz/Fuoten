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
import harbour.fuoten.items 1.0
import "../../common/parts"


SilicaListView {
    id: feedListFlick
    anchors.fill: parent
    currentIndex: -1

    property string searchString
    property Page page: null
    property bool searchVisible: false
    property Folder folder: null

    ContextConfig {
        id: feedContextConfig
        contextType: !folder ? FuotenApp.StartPage : FuotenApp.Feeds
    }

    Component.onCompleted: {
        if (!page.forwardNavigation && page.status === PageStatus.Active) {
            pageStack.pushAttached(Qt.resolvedUrl("../../common/pages/ContextConfigPage.qml"), {cc: feedContextConfig, name: folder ? folder.name : ""})
        }
    }

    Connections {
        target: page
        onStatusChanged: {
            if (page.status === PageStatus.Active && !page.forwardNavigation) {
                pageStack.pushAttached(Qt.resolvedUrl("../../common/pages/ContextConfigPage.qml"), {cc: feedContextConfig, name: folder ? folder.name : ""})
            }
        }
    }

    PullDownMenu {
        busy: synchronizer.inOperation

        MenuItem {
            //% "About"
            text: qsTrId("id-about")
            onClicked: pageStack.push(Qt.resolvedUrl("../../common/pages/About.qml"))
            visible: !folder
        }

        MenuItem {
            //% "Settings"
            text: qsTrId("id-settings")
            onClicked: pageStack.push(Qt.resolvedUrl("../../common/pages/Settings.qml"))
            visible: !folder
        }

        MenuItem {
            visible: folder
            //% "Mark folder read"
            text: qsTrId("fuoten-mark-folder-read")
            enabled: !folder.inOperation && folder.unreadCount > 0
            onClicked: //% "Marking %1 read"
                       remorsePop.execute(qsTrId("fuoten-marking-read").arg(folder.name), function() {folder.markAsRead(config, localstorage)})
        }

        MenuItem {
            text: feedListFlick.searchVisible
                    //% "Hide search"
                  ? qsTrId("fuoten-hide-search")
                    //% "Show search"
                  : qsTrId("fuoten-show-search")
            onClicked: feedListFlick.searchVisible = !feedListFlick.searchVisible
        }

        MenuItem {
            //% "Synchronize"
            text: qsTrId("fuoten-synchronize")
            onClicked: synchronizer.sync()
            enabled: !synchronizer.inOperation
        }
    }

    VerticalScrollDecorator { flickable: feedListFlick; page: feedListFlick.page }

    header: ListPageHeader {
        id: feedsListHeader
        headerTitle: feedListFlick.folder ? feedListFlick.folder.name : "Fuoten"
        //% "%n feed(s)"
        headerDescription: feedListFlick.folder ? qsTrId("fuoten-feeds-count", feedListFlick.folder.feedCount) : ""
        page: feedListFlick.page
        searchVisible: feedListFlick.searchVisible
        startPage: !folder
        folders: false
        feedListPage: folder !== null
        onSearchTextChanged: feedListFlick.searchString = searchText
    }

    model: FeedListFilterModel {
        id: feedListModel
        parentId: feedListFlick.folder ? feedListFlick.folder.id : -1
        storage: localstorage
        sortingRole: feedContextConfig.sorting
        search: feedListFlick.searchString
        hideRead: feedContextConfig.hideRead
        sortOrder: feedContextConfig.sortOrder
        respectPinned: feedContextConfig.respectPinned
        sortByFolder: feedContextConfig.showFolderSections
        Component.onCompleted: load(config.language)
    }

    section {
        property: 'display.folderName'
        delegate: feedContextConfig.showFolderSections && !folder ? secHeader : null
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
            visible: !model.display.error

            Image {
                Layout.preferredHeight: Theme.iconSizeSmall
                Layout.preferredWidth: Theme.iconSizeSmall
                source: model.display.faviconLink
            }

            Label {
                Layout.fillWidth: true
                font.pixelSize: Theme.fontSizeMedium
                text: Theme.highlightText(model.display ? model.display.title : "", feedListFlick.searchString, Theme.highlightColor)
                truncationMode: TruncationMode.Fade
                color: feedListItem.highlighted ? (model.display.unreadCount ? Theme.highlightColor : Theme.secondaryHighlightColor) : (model.display.unreadCount ? Theme.primaryColor : Theme.secondaryColor)
                textFormat: Text.StyledText
            }

            Label {
                text: model.display.unreadCount
                color: model.display.unreadCount ? Theme.highlightColor : feedListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                visible: !model.display.inOperation
                font.pixelSize: Theme.fontSizeMedium
            }

            BusyIndicator {
                size: BusyIndicatorSize.Small
                visible: model.display.inOperation
                running: model.display.inOperation
            }
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
            }
        }
    }

    RemorsePopup {
        id: remorsePop
    }

    Component {
        id: secHeader
        SectionHeader {
            text: section
            visible: text != ""
            height: Theme.itemSizeExtraSmall
        }
    }
}

