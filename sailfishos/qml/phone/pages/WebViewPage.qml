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
import harbour.fuoten.items 1.0

Page {
    id: articleWebView

    property Article article: null
    property bool isAttached: false
    property bool loaded: false

    onStatusChanged: {
        if (status === PageStatus.Active) {
            if (!loaded && article) {
                webViewLoader.sourceComponent = webViewComponent
                loaded = true
            }
        }
    }

    Component.onCompleted: {
        cc.article = article
    }

    Component.onDestruction: {
        if (!isAttached) {
            cc.article = null
        }
    }

    Loader {
        id: webViewLoader
        anchors.fill: parent
    }

    Component {
        id: webViewComponent

        Item {
            anchors.fill: parent

            SilicaWebView {
                id: webView
                anchors { left: parent.left; right: parent.right; bottom: navBar.top; top: parent.top }

                url: article.url

                Component.onCompleted: {
                    experimental.userAgent = "Mozilla/5.0 (Maemo; Linux; Jolla; Sailfish; Mobile) AppleWebKit/534.13 (KHTML, like Gecko) NokiaBrowser/8.5.0 Mobile Safari/534.13";
                    experimental.preferences.defaultFontSize = Theme.fontSizeSmall
                    experimental.preferences.minimumFontSize = Theme.fontSizeExtraSmall
                    experimental.preferences.pluginsEnabled = true
                    experimental.preferences.javascriptEnabled = true
                }

                PullDownMenu {
                    MenuItem {
                        text: article && article.starred
                                //% "Remove from favorites"
                              ? qsTrId("fuoten-remove-from-favorites")
                                //% "Add to favorites"
                              : qsTrId("fuoten-add-to-favorites")
                        onClicked: article.star(!article.starred, config, localstorage, true)
                        enabled: !article.inOperation
                    }
                    MenuItem {
                        text: article && article.unread
                                //% "Mark as read"
                              ? qsTrId("fuoten-mark-item-as-read")
                                //% "Mark as unread"
                              : qsTrId("fuoten-mark-item-as-unread")
                        onClicked: article.mark(!article.unread, config, localstorage, true)
                        enabled: !article.inOperation
                    }

                    MenuItem {
                        //% "Share"
                        text: qsTrId("fuoten-share")
                        enabled: article
                        onClicked: pageStack.push(Qt.resolvedUrl("../../common/pages/Sharing.qml"), {"shareUrl": article.url.toString(), "shareTitle": article.title })
                    }

                    MenuItem {
                        //% "Open in browser"
                        text: qsTrId("fuoten-open-in-browser")
                        onClicked: Qt.openUrlExternally(article.url)
                    }

                    MenuItem {
                        //% "Back"
                        text: qsTrId("fuoten-back")
                        onClicked: pageStack.navigateBack(PageStackAction.Animated)
                    }
                }
            }

            Item {
                id: navBar
                anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
                height: Theme.itemSizeSmall

                Rectangle {
                    height: parent.height
                    anchors.left: parent.left
                    width: webView.loadProgress < 100 ? (parent.width * (webView.loadProgress/100)) : 0
                    color: Theme.rgba(Theme.highlightColor, 0.3)
                }

                IconButton {
                    width: Theme.itemSizeMedium; height: Theme.itemSizeMedium
                    icon.source: "image://theme/icon-m-back"
                    enabled: webView.canGoBack
                    anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
                    onClicked: webView.goBack()
                }

                IconButton {
                    width: Theme.itemSizeMedium; height: Theme.itemSizeMedium
                    icon.source: webView.loading ? "image://theme/icon-m-reset" : "image://theme/icon-m-refresh"
                    anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
                    onClicked: webView.loading ? webView.stop() : webView.reload()
                }

                IconButton {
                    width: Theme.itemSizeMedium; height: Theme.itemSizeMedium
                    icon.source: "image://theme/icon-m-forward"
                    enabled: webView.canGoForward
                    anchors { right: parent.right; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
                    onClicked: webView.goForward()
                }
            }
        }
    }
}
