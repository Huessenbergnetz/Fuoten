/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/qml/phone/pages/WebViewPage.qml
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

Page {
    id: articleWebView

    property Article article: null
    property ContextConfig cc: null
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
        covercon.article = article
    }

    Component.onDestruction: {
        if (!isAttached) {
            covercon.article = null
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


                Component.onCompleted: {
                    experimental.userAgent = cc ? cc.userAgent : "Mozilla/5.0 (Maemo; Linux; U; Jolla; Sailfish; like Android) AppleWebKit/538.1 (KHTML, like Gecko) Version/5.1 Chrome/30.0.0.0 Mobile Safari/538.1 (compatible)"
                    experimental.preferences.defaultFontSize = (cc && cc.defaultFontSize > 0) ? cc.defaultFontSize : Theme.fontSizeSmall
                    experimental.preferences.minimumFontSize = (cc && cc.minimumFontSize > 0) ? cc.minimumFontSize : Theme.fontSizeExtraSmall
                    experimental.preferences.pluginsEnabled = true
                    experimental.preferences.javascriptEnabled = true
                    url = article.url
                }

                PullDownMenu {
                    MenuItem {
                        text: article && article.starred
                              ? qsTrId("fuoten-remove-from-favorites")
                              : qsTrId("fuoten-add-to-favorites")
                        onClicked: article.star(!article.starred, config, localstorage, true)
                        enabled: !article.inOperation
                    }
                    MenuItem {
                        text: article && article.unread
                              ? qsTrId("fuoten-mark-item-as-read")
                              : qsTrId("fuoten-mark-item-as-unread")
                        onClicked: article.mark(!article.unread, config, localstorage, true)
                        enabled: !article.inOperation
                    }

                    MenuItem {
                        text: qsTrId("fuoten-share")
                        enabled: article
                        onClicked: pageStack.push(Qt.resolvedUrl("../../common/pages/Sharing.qml"), {"shareUrl": article.url.toString(), "shareTitle": article.title })
                    }

                    MenuItem {
                        text: qsTrId("fuoten-open-in-browser")
                        onClicked: Qt.openUrlExternally(article.url)
                    }

                    MenuItem {
                        //: Menu entry on the web view to go back to the previous appliction page
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
