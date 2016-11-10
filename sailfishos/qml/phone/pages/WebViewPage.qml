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

    Loader {
        id: webViewLoader
        anchors.fill: parent
        sourceComponent: ((articleWebView.status === PageStatus.Active) && article) ? webViewComponent : undefined
    }

    Component {
        id: webViewComponent

        SilicaWebView {
            id: webView
            anchors.fill: parent

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
                    //% "Copy URL"
                    text: qsTrId("fuoten-copy-url")
                    enabled: article
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
    }
}
