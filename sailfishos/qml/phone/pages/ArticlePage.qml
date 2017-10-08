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
import harbour.fuoten.items 1.0
import "../../common/parts"

Page {
    id: articlePage

    property Article article: null
    property ContextConfig cc: null

    onStatusChanged: {
        if (status === PageStatus.Active) {
            if (!articlePage.canNavigateForward) {
                pageStack.pushAttached(Qt.resolvedUrl("WebViewPage.qml"), {article: article, isAttached: true, })
            }
        }
    }

    Component.onCompleted: {
        covercon.article = article
    }

    Component.onDestruction: {
        covercon.article = null;
    }

    SilicaFlickable {
        id: articleFlick
        anchors.fill: parent
        contentHeight: headerCol.height + contentText.height + Theme.paddingLarge

        VerticalScrollDecorator { page: articlePage; flickable: articleFlick }

        PullDownMenu {
            busy: synchronizer.inOperation || (article && article.inOperation)
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
                enabled: article
            }
        }

        PushUpMenu {
            MenuItem {
                //% "Scroll to top"
                text: qsTrId("fuoten-scroll-to-top")
                visible: articleFlick.contentHeight > articleFlick.height
                onClicked: articleFlick.scrollToTop()
            }
            MenuItem {
                //% "Open in browser"
                text: qsTrId("fuoten-open-in-browser")
                onClicked: Qt.openUrlExternally(article.url)
                enabled: article
            }
        }

        ColumnLayout {
            id: headerCol
            spacing: Theme.paddingSmall
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }

            PageHeader { page: articlePage; title: article ? article.feedTitle : ""; leftMargin: 0; rightMargin: 0; Layout.fillWidth: true }

            RowLayout {
                spacing: Theme.paddingSmall
                Layout.fillWidth: true

                Label {
                    Layout.fillWidth: true
                    text: article.title
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    textFormat: Text.PlainText
                    color: Theme.highlightColor
                }

                ColumnLayout {
                    Layout.preferredWidth: Theme.iconSizeSmall
                    Layout.alignment: Qt.AlignTop | Qt.AlignLeft

                    Image {
                        opacity: article.starred ? 1.0 : 0.0
                        Layout.preferredWidth: Theme.iconSizeSmall
                        Layout.preferredHeight: Theme.iconSizeSmall
                        source: "image://theme/icon-s-favorite"
                        Behavior on opacity { FadeAnimator{} }
                    }
                }
            }

            Row {
                spacing: Theme.paddingSmall
                Layout.fillWidth: true

                Text {
                    id: pdString
                    text: article.humanPubDate
                    font.pixelSize: Theme.fontSizeExtraSmall
                    textFormat: Text.PlainText
                    color: Theme.highlightColor
                }

                Text {
                    id: pdSep
                    text: "|"
                    font.pixelSize: Theme.fontSizeExtraSmall
                    textFormat: Text.PlainText
                    color: Theme.highlightColor
                }

                Text {
                    id: ptString
                    text: article.humanPubTime
                    font.pixelSize: Theme.fontSizeExtraSmall
                    textFormat: Text.PlainText
                    color: Theme.highlightColor
                }

                Text {
                    id: ptSep
                    text: "|"
                    font.pixelSize: Theme.fontSizeExtraSmall
                    textFormat: Text.PlainText
                    color: Theme.highlightColor
                    visible: article.author.length > 0
                }

                Label {
                    text: article.author
                    font.pixelSize: Theme.fontSizeExtraSmall
                    textFormat: Text.PlainText
                    color: Theme.highlightColor
                    truncationMode: TruncationMode.Fade
                    width: parent.width - pdString.width - pdSep.width - ptString.width - ptSep.width - (parent.spacing * 4)
                }
            }

            Separator {
                Layout.fillWidth: true
                color: Theme.primaryColor
            }
        }

        RescalingRichText {
            id: contentText
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; top: headerCol.bottom }
            color: Theme.primaryColor
            Component.onCompleted: text = localstorage.getArticleBody(article.id)
            onLinkActivated: Qt.openUrlExternally(link)
        }
    }
}

