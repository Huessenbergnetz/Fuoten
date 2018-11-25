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
import Sailfish.Silica 1.0
import harbour.fuoten 1.0
import harbour.fuoten.items 1.0

SilicaListView {
    id: baseListView
    anchors.fill: parent
    currentIndex: -1

    property string searchString
    property bool searchVisible: false
    property Page page: null
    property Feed feed: null
    property Folder folder: null
    property alias contextType: contextConfig.contextType
    property bool inOperation: true
    property alias noContentText: emptyContent.text
    property bool startPage: false
    property alias cc: contextConfig

    property string _contextName: ""

    clip: synchronizer.inOperation || synchronizer.error

    ContextConfig {
        id: contextConfig
        contextId: feed ? feed.id
                          : folder
                            ? folder.id
                            : -1
    }

    function setContextName() {
        _contextName = feed
                       ? feed.title
                       : folder
                         ? folder.name
                         : contextType === FuotenApp.AllItems
                           ? qsTrId("fuoten-all-articles")
                           : contextType === FuotenApp.StarredItems
                             ? qsTrId("fuoten-starred-articles")
                             : ""
    }

    Component.onCompleted: {
        if (!page.forwardNavigation && page.status === PageStatus.Active) {
            if (_contextName.length === 0) {
                setContextName()
            }
            pageStack.pushAttached(Qt.resolvedUrl("../../common/pages/ContextConfigPage.qml"), {cc: contextConfig, name: _contextName})
        }
    }

    Connections {
        target: page
        onStatusChanged: {
            if (page.status === PageStatus.Active && !page.forwardNavigation) {
                if (_contextName.length === 0) {
                    setContextName()
                }
                pageStack.pushAttached(Qt.resolvedUrl("../../common/pages/ContextConfigPage.qml"), {cc: contextConfig, name: _contextName})
            }
        }
    }



    PullDownMenu {
        id: baseListViewPullDown
        busy: synchronizer.inOperation || (feed && feed.inOperation) || (folder && folder.inOperation) || localstorage.inOperation || baseListView.inOperation
        property string lastSyncString: config.getHumanLastSync()
        onActiveChanged: if(active) { lastSyncString = config.getHumanLastSync() }
        flickable: baseListView

        MenuItem {
            text: qsTrId("id-about")
            onClicked: pageStack.push(Qt.resolvedUrl("../../common/pages/About.qml"))
            visible: startPage
        }

        MenuItem {
            text: qsTrId("id-settings")
            onClicked: pageStack.push(Qt.resolvedUrl("../../common/pages/Settings.qml"))
            visible: startPage
        }

        MenuItem {
            visible: folder
            text: qsTrId("fuoten-mark-folder-read")
            enabled: folder && !folder.inOperation && folder.unreadCount > 0
            onClicked: {
                //: Description of a remorse popup to mark a folder as read, %1 will be the name of the folder
                //% "Marking %1 read"
                remorsePop.execute(qsTrId("fuoten-marking-read").arg(folder.name), function() {folder.markAsRead(config, localstorage, true)})
            }
        }

        MenuItem {
            visible: feed
            //: Menu entry for marking all articles in a feed as read
            //% "Mark feed read"
            text: qsTrId("fuoten-mark-feed-read")
            enabled: feed && !feed.inOperation && feed.unreadCount > 0
            onClicked: feed.markAsRead(config, localstorage, true)
        }

        MenuItem {
            visible: !feed && !folder && contextType === FuotenApp.AllItems
            enabled: !localstorage.inOperation
            //: Menu entry for marking all articles as read
            //% "Mark all as read"
            text: qsTrId("fuoten-mark-all-read")
            //: Remorse popup description for marking all articles as read
            //% "Marking all read"
            onClicked: remorsePop.execute(qsTrId("fuoten-marking-all-read"), function() {localstorage.enqueueMarkAllItemsRead()})
        }

        MenuItem {
            text: baseListView.searchVisible
                    //: Pull down menu entry to hide the search input field
                    //% "Hide search"
                  ? qsTrId("fuoten-hide-search")
                    //: Pull down menu entry to show the search input field
                    //% "Show search"
                  : qsTrId("fuoten-show-search")
            onClicked: baseListView.searchVisible = !baseListView.searchVisible
        }

        MenuItem {
            //: Menu entry to start synchronizing
            //% "Synchronize"
            text: qsTrId("fuoten-synchronize")
            onClicked: synchronizer.sync()
            enabled: !synchronizer.inOperation
        }

        MenuLabel {
            text: synchronizer.inOperation
                  ? qsTrId("fuoten-synchronizing")
                    //: Text in the menu showing the last sync time. %1 will contain something like "11 minutes ago"
                    //% "Last synchronization: %1"
                  : qsTrId("fuoten-last-sync-time").arg(baseListViewPullDown.lastSyncString)
        }
    }


    VerticalScrollDecorator { flickable: baseListView; page: baseListView.page }

    RemorsePopup {
        id: remorsePop
    }

    BusyIndicator {
        anchors.centerIn: parent
        size: BusyIndicatorSize.Large
        visible: baseListView.inOperation
        running: baseListView.inOperation
    }

    ViewPlaceholder {
        id: invalidAccountPlaceHolder
        flickable: baseListView
        enabled: baseListView.count === 0 && !config.isAccountValid && !baseListView.inOperation
        //: text on a view placeholder
        //% "Invalid account configuration"
        text: qsTrId("fuoten-invalid-account")
        //: hint on a view placeholder
        //% "Open the settings to configure your account"
        hintText: qsTrId("fuoten-invalid-account-hint")
    }

    ViewPlaceholder {
        id: emptyContent
        flickable: baseListView
        enabled: baseListView.count === 0 && config.isAccountValid && !baseListView.inOperation
        //: Placeholder hint on a view if no content has been found
        //% "Synchronize your data or check your filter settings."
        hintText: qsTrId("fuoten-no-content-found-hint")
    }
}
