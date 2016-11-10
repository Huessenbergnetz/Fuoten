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
    id: feedListView
    anchors.fill: parent
    currentIndex: -1

    property string searchString
    property Page page: null
    property bool searchVisible: false
    property Folder folder: null

    ContextConfig {
        id: feedContextConfig
        contextType: !folder ? FuotenApp.StartPage : FuotenApp.Feeds
        contextId: folder ? folder.id : -1
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
        id: feedListViewPullDown
        busy: synchronizer.inOperation || (folder && folder.inOperation)
        property string lastSyncString: config.getHumanLastSync()

        onActiveChanged: lastSyncString = config.getHumanLastSync()

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
            enabled: folder && !folder.inOperation && folder.unreadCount > 0
            onClicked: //% "Marking %1 read"
                       remorsePop.execute(qsTrId("fuoten-marking-read").arg(folder.name), function() {folder.markAsRead(config, localstorage, true)})
        }

        MenuItem {
            text: feedListView.searchVisible
                    //% "Hide search"
                  ? qsTrId("fuoten-hide-search")
                    //% "Show search"
                  : qsTrId("fuoten-show-search")
            onClicked: feedListView.searchVisible = !feedListView.searchVisible
        }

        MenuItem {
            //% "Synchronize"
            text: qsTrId("fuoten-synchronize")
            onClicked: synchronizer.sync()
            enabled: !synchronizer.inOperation
        }

        MenuLabel {
            text: synchronizer.inOperation
                    //% "Synchronizing"
                  ? qsTrId("fuoten-synchronizing")
                    //: %1 will contain something like "11 minutes"
                    //% "Last synchronization: %1 ago"
                  : qsTrId("fuoten-last-sync-time").arg(feedListViewPullDown.lastSyncString)
        }
    }

    VerticalScrollDecorator { flickable: feedListView; page: feedListView.page }

    header: ListPageHeader {
        id: feedsListHeader
        page: feedListView.page
        searchVisible: feedListView.searchVisible
        folders: false
        folder: feedListView.folder
        onSearchTextChanged: feedListView.searchString = searchText
        onAllArticlesClicked: folder ? pageStack.push(Qt.resolvedUrl("ArticlesListPage.qml"), {context: FuotenApp.FolderItems, folder: folder}) : pageStack.push(Qt.resolvedUrl("ArticlesListPage.qml"), {context: FuotenApp.AllItems})
        onStarredItemsClicked: pageStack.push(Qt.resolvedUrl("ArticlesListPage.qml"), {context: FuotenApp.StarredItems})
    }

    model: FeedListFilterModel {
        id: feedListModel
        parentId: feedListView.folder ? feedListView.folder.id : -1
        storage: localstorage
        sortingRole: feedContextConfig.sorting
        search: feedListView.searchString
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

    delegate: FeedListDelegate { searchString: feedListView.searchString }

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

    BusyIndicator {
        anchors.centerIn: parent
        size: BusyIndicatorSize.Large
        visible: feedListModel.inOperation
        running: feedListModel.inOperation
    }

    ViewPlaceholder {
        id: invalidAccountPlaceHolder
        flickable: feedListView
        enabled: feedListView.count === 0 && !config.isAccountValid && !feedListModel.inOperation
        //% "Invalid account configuration"
        text: qsTrId("fuoten-invalid-account")
        //% "Open the settings to configure your account"
        hintText: qsTrId("fuoten-invalid-account-hint")
    }

    ViewPlaceholder {
        id: emptyContent
        flickable: feedListView
        enabled: feedListView.count === 0 && config.isAccountValid && !feedListModel.inOperation
        //% "No feeds found"
        text: qsTrId("fuoten-no-feeds-found")
        //% "Synchronize your data or check your filter settings."
        hintText: qsTrId("fuoten-no-content-found-hint")
    }
}

