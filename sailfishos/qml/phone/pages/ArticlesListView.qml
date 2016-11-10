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
    id: articlesListView
    anchors.fill: parent
    currentIndex: -1

    property string searchString
    property bool searchVisible: false
    property Page page: null
    property Feed feed: null
    property Folder folder: null
    property alias contextType: articlesContextConfig.contextType

    property string _contextName: ""

    ContextConfig {
        id: articlesContextConfig
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
            pageStack.pushAttached(Qt.resolvedUrl("../../common/pages/ContextConfigPage.qml"), {cc: articlesContextConfig, name: _contextName})
        }
    }

    Connections {
        target: page
        onStatusChanged: {
            if (page.status === PageStatus.Active && !page.forwardNavigation) {
                if (_contextName.length === 0) {
                    setContextName()
                }
                pageStack.pushAttached(Qt.resolvedUrl("../../common/pages/ContextConfigPage.qml"), {cc: articlesContextConfig, name: _contextName})
            }
        }
    }

    PullDownMenu {
        id: articlesListViewPullDown
        busy: synchronizer.inOperation || (feed && feed.inOperation) || (folder && folder.inOperation)
        property string lastSyncString: config.getHumanLastSync()

        onActiveChanged: lastSyncString = config.getHumanLastSync()

        MenuItem {
            visible: folder
            //% "Mark folder read"
            text: qsTrId("fuoten-mark-folder-read")
            enabled: folder && !folder.inOperation && folder.unreadCount > 0
            onClicked: //% "Marking %1 read"
                       remorsePop.execute(qsTrId("fuoten-marking-read").arg(folder.name), function() {folder.markAsRead(config, localstorage)})
        }

        MenuItem {
            visible: feed
            //% "Mark feed read"
            text: qsTrId("fuoten-mark-feed-read")
            enabled: feed && !feed.inOperation && feed.unreadCount > 0
            onClicked: feed.markAsRead(config, localstorage, true)
        }

        MenuItem {
            visible: !feed && !folder && contextType === FuotenApp.AllItems
            //% "Mark all as read"
            text: qsTrId("fuoten-mark-all-read")
            //% "Marking all read"
            onClicked: remorsePop.execute(qsTrId("fuoten-marking-all-read"), function() {localstorage.enqueueMarkAllItemsRead()})
        }

        MenuItem {
            text: articlesListView.searchVisible
                    //% "Hide search"
                  ? qsTrId("fuoten-hide-search")
                    //% "Show search"
                  : qsTrId("fuoten-show-search")
            onClicked: articlesListView.searchVisible = !articlesListView.searchVisible
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
                  : qsTrId("fuoten-last-sync-time").arg(articlesListViewPullDown.lastSyncString)
        }
    }

    VerticalScrollDecorator { flickable: articlesListView; page: articlesListView.page }

    header: ListPageHeader {
        id: articlesListHeader
        page: articlesListView.page
        searchVisible: articlesListView.searchVisible
        folders: false
        folder: articlesListView.folder
        folderItems: contextType === FuotenApp.FolderItems
        feed: articlesListView.feed
        onSearchTextChanged: articlesListView.searchString = searchText
        Component.onCompleted: {
            if (contextType === FuotenApp.AllItems) {
                startPage = false
                title = qsTrId("fuoten-all-articles")
            } else if (contextType === FuotenApp.StarredItems) {
                startPage = false
                title = qsTrId("fuoten-starred-articles")
            }
        }
    }

    model: ArticleListFilterModel {
        id: articlesModel
        storage: localstorage
        sortOrder: articlesContextConfig.sortOrder
        hideRead: articlesContextConfig.hideRead
        search: articlesListView.searchString
        bodyLimit: articlesContextConfig.showExcerpt ? 250 : -1
        Component.onCompleted: {
            if (feed) {
                parentId = feed.id
                parentIdType = Fuoten.Feed
            } else if (folder) {
                parentId = folder.id
                parentIdType = Fuoten.Folder
            } else if (contextType === FuotenApp.StarredItems) {
                parentIdType = Fuoten.Starred
            }
            load()
        }
        onBodyLimitChanged: bodyLimitChangedTimer.start()
    }

    Timer {
        id: bodyLimitChangedTimer
        interval: 200
        onTriggered: {
            if (!articlesModel.inOperation) {
                articlesModel.reload()
            }
        }
    }

    delegate: ListItem {
        id: articleListItem

        contentHeight: !display.error ? Math.max(textCol.height, iconCol.height) + Theme.paddingSmall : errorItem.height + Theme.paddingSmall
        contentWidth: parent.width

        ListView.onAdd: AddAnimation { target: articleListItem }
        ListView.onRemove: RemoveAnimation { target: articleListItem }

        menu: itemContextMenu

        onClicked: {
            if (display.error) {
                display.clearError()
            } else {
                switch(articlesContextConfig.openArticles) {
                case FuotenApp.OpenInternal:
                    pageStack.push(Qt.resolvedUrl("ArticlePage.qml"), {article: display})
                    break
                case FuotenApp.OpenWebView:
                    pageStack.push(Qt.resolvedUrl("WebViewPage.qml"), {article: display})
                    break;
                default:
                    Qt.openUrlExternally(display.url)
                }

                if (display.unread) {
                    display.mark(false, config, localstorage, true)
                }
            }
        }

        Item {
            width: gi.width
            height: gi.height
            x: -(width/2)
            y: -(height/4)

            GlassItem {
                id: gi
                width: Theme.itemSizeExtraSmall
                height: Theme.itemSizeExtraSmall
                color: Theme.highlightColor
                opacity: display.unread ? 1 : 0
                Behavior on opacity { NumberAnimation { duration: 250; easing.type: Easing.InOutQuad } }
            }
        }

        ErrorItem {
            id: errorItem
            error: display.error
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
            highlighted: articleListItem.highlighted
            fontSize: Theme.fontSizeExtraSmall
        }

        RowLayout {
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
            spacing: Theme.paddingSmall
            visible: !display.error

            ColumnLayout {
                id: textCol
                Layout.fillWidth: true
                spacing: 0

                Text {
                    id: titleText
                    text: Theme.highlightText(display ? display.title : "", articlesListView.searchString, Theme.highlightColor)
                    Layout.fillWidth: true
                    color: articleListItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                    maximumLineCount: 3
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    elide: Text.ElideRight
                    textFormat: Text.StyledText
                    font.pixelSize: Theme.fontSizeSmall
                }

                Text {
                    id: excerptText
                    visible: articlesContextConfig.showExcerpt && display.body.length > 0
                    text: display.body
                    Layout.fillWidth: true
                    color: articleListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    maximumLineCount: 5
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    elide: Text.ElideRight
                    textFormat: Text.PlainText
                    font.pixelSize: Theme.fontSizeExtraSmall
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: Theme.paddingSmall

                    Text {
                        text: display.humanPubDate
                        textFormat: Text.PlainText
                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeTiny
                    }

                    Text {
                        text: "|"
                        textFormat: Text.PlainText
                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeTiny
                    }

                    Text {
                        text: display.humanPubTime
                        textFormat: Text.PlainText
                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeTiny
                    }

                    Text {
                        text: "|"
                        textFormat: Text.PlainText
                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeTiny
                        visible: articlesListView.contextType !== FuotenApp.FeedItems
                    }

                    Text {
                        text: display.feedTitle
                        textFormat: Text.PlainText
                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeTiny
                        visible: articlesListView.contextType !== FuotenApp.FeedItems
                    }
                }
            }

            ColumnLayout {
                id: iconCol
                Layout.preferredWidth: Theme.iconSizeSmall
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter

                Image {
                    id: starImage
                    opacity: display.starred ? 1.0 : 0.0
                    Layout.preferredWidth: Theme.iconSizeSmall
                    Layout.preferredHeight: Theme.iconSizeSmall
                    source: "image://theme/icon-s-favorite?" + (articleListItem.highlighted ? Theme.highlightColor : Theme.primaryColor)
                    Behavior on opacity { FadeAnimator {} }
                }
            }
        }

        Component {
            id: itemContextMenu
            ContextMenu {
                MenuItem {
                    text: display.unread
                            //% "Mark as read"
                          ? qsTrId("fuoten-mark-item-as-read")
                            //% "Mark as unread"
                          : qsTrId("fuoten-mark-item-as-unread")
                    onClicked: display.mark(!display.unread, config, localstorage, true)
                }
                MenuItem {
                    text: display.starred
                            //% "Remove from favorites"
                          ? qsTrId("fuoten-remove-from-favorites")
                            //% "Add to favorites"
                          : qsTrId("fuoten-add-to-favorites")
                    onClicked: display.star(!display.starred, config, localstorage, true)
                }
            }
        }
    }

    RemorsePopup {
        id: remorsePop
    }

    BusyIndicator {
        anchors.centerIn: parent
        size: BusyIndicatorSize.Large
        visible: articlesModel.inOperation
        running: articlesModel.inOperation
    }

    ViewPlaceholder {
        id: emptyContent
        flickable: articlesListView
        enabled: articlesListView.count === 0 && config.isAccountValid && !articlesModel.inOperation
        //% "No articles found"
        text: qsTrId("fuoten-no-articles-found")
        //% "Synchronize your data or check your filter settings."
        hintText: qsTrId("fuoten-no-content-found-hint")
    }
}
