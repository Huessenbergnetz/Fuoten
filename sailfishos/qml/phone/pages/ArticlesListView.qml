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

BaseListView {
    id: articlesListView

    inOperation: ((page.status === PageStatus.Activating) && !articlesModel.loaded) || articlesModel.inOperation

    //% "No articles found"
    noContentText: qsTrId("fuoten-no-articles-found")

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

    Connections {
        target: articlesListView.page
        onStatusChanged: {
            if ((status === PageStatus.Active) && !articlesModel.inOperation && !articlesModel.loaded) {
                articlesModel.load()
            }
        }
    }

    model: ArticleListFilterModel {
        id: articlesModel
        storage: localstorage
        sortOrder: articlesListView.cc.sortOrder
        hideRead: articlesListView.cc.hideRead
        search: articlesListView.searchString
        bodyLimit: articlesListView.cc.showExcerpt ? 250 : -1
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
        }
        onBodyLimitChanged: {if (articlesModel.loaded) {bodyLimitChangedTimer.start()}}
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

        contentHeight: display && display.error ? errorItem.height + Theme.paddingSmall : Math.max(textCol.height, iconCol.height) + Theme.paddingSmall
        contentWidth: parent.width

        ListView.onAdd: AddAnimation { target: articleListItem }
        ListView.onRemove: RemoveAnimation { target: articleListItem }

        menu: itemContextMenu

        onClicked: {
            if (display.error) {
                display.clearError()
            } else {
                switch(articlesListView.cc.openArticles) {
                case FuotenApp.OpenInternal:
                    pageStack.push(Qt.resolvedUrl("ArticlePage.qml"), {article: display, cc: articlesListView.cc})
                    break
                case FuotenApp.OpenWebView:
                    pageStack.push(Qt.resolvedUrl("WebViewPage.qml"), {article: display, cc: articlesListView.cc})
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
                    visible: articlesListView.cc.showExcerpt && display.body.length > 0
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
                MenuItem {
                    text: qsTrId("fuoten-share")
                    onClicked: pageStack.push(Qt.resolvedUrl("../../common/pages/Sharing.qml"), {"shareUrl": display.url.toString(), "shareTitle": display.title })
                }
            }
        }
    }
}
