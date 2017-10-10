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
import "../parts"

Page {
    id: contextConfigPage

    property ContextConfig cc: null
    property alias name: contextHeader.description

    SilicaFlickable {
        id: contextConfigFlick
        anchors.fill: parent
        contentHeight: contextConfigGrid.height

        PullDownMenu {
            MenuItem {
                //% "Create folder"
                text: qsTrId("fuoten-create-folder")
                onClicked: pageStack.push(Qt.resolvedUrl("../dialogs/CreateFolderDialog.qml"))
            }
            MenuItem {
                //% "Add feed"
                text: qsTrId("fuoten-add-feed")
                onClicked: pageStack.push(Qt.resolvedUrl("../dialogs/CreateFeedDialog.qml"))
            }
        }

        VerticalScrollDecorator { flickable: contextConfigFlick; page: contextConfigPage }

        GridLayout {
            id: contextConfigGrid
            anchors { left: parent.left; right: parent.right }

            columnSpacing: 0
            rowSpacing: Theme.paddingSmall

            columns: Screen.sizeCategory < Screen.Large ? (contextConfigPage.isLandscape ? 2 : 1) : (contextConfigPage.isLandscape ? 4 : 2)

            PageHeader {
                id: contextHeader
                //% "Main view settings"
                title: cc.contextType === FuotenApp.StartPage
                       ? qsTrId("fuoten-mainview-settings")
                       : cc.contextType === FuotenApp.Feeds
                         //% "Folder settings"
                         ? qsTrId("fuoten-folder-settings")
                         //% "Articles list settings"
                         : qsTrId("fuoten-articles-list-settings")
                page: contextConfigPage
                Layout.columnSpan: contextConfigGrid.columns
                Layout.fillWidth: true
            }

            SectionHeader {
                //% "List appearance"
                text: qsTrId("fuoten-config-section-list-appearance")
                Layout.columnSpan: contextConfigGrid.columns
                Layout.preferredWidth: contextConfigGrid.width - Theme.horizontalPageMargin
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: sortingChoser.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.columnSpan: contextConfigGrid.columns
                visible: cc.contextType < FuotenApp.AllItems

                ComboBox {
                    id: sortingChoser
                    //% "Sort by"
                    label: qsTrId("fuoten-sort-by-label")
                    //% "Choose the role you want to sort the content by."
                    description: qsTrId("fuoten-sort-by-desc")
                    menu: ContextMenu {
                        MenuItem {
                            //% "Database ID"
                            text: qsTrId("fuoten-sort-database-id")
                            readonly property int value: Fuoten.ID
                        }
                        MenuItem {
                            //% "Title"
                            text: qsTrId("fuoten-sort-title")
                            readonly property int value: Fuoten.Name
                        }
                        MenuItem {
                            //% "Time"
                            text: qsTrId("fuoten-sort-time")
                            readonly property int value: Fuoten.Time
                            visible: cc.contextType > FuotenApp.Feeds
                        }
                        MenuItem {
                            //% "Unread count"
                            text: qsTrId("fuoten-sort-unread-count")
                            readonly property int value: Fuoten.UnreadCount
                        }
                        MenuItem {
                            //% "Feed count"
                            text: qsTrId("fuoten-sort-feed-count")
                            readonly property int value: Fuoten.FeedCount
                            visible: (cc.contextType === FuotenApp.Folders) || (cc.contextType === FuotenApp.StartPage && config.mainViewType === Fuoten.Folder)
                        }
                    }

                    onCurrentIndexChanged: if (currentItem) { cc.sorting = currentItem.value }
                    currentIndex: cc.sorting
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: sortOrderSwitch.implicitHeight
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                TextSwitch {
                    id: sortOrderSwitch
                    automaticCheck: false
                    //% "Sort descending"
                    text: qsTrId("fuoten-sort-descending-label")
                    //% "If enabled, depending on the context, the feeds, folders or articles will be sorted in descending order."
                    description: qsTrId("fuoten-sort-descending-desc")
                    checked: cc.sortOrder === Qt.DescendingOrder
                    onClicked: {
                        if (cc.sortOrder === Qt.DescendingOrder) {
                            cc.sortOrder = Qt.AscendingOrder
                        } else {
                            cc.sortOrder = Qt.DescendingOrder
                        }
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: hideReadSwitch.implicitHeight
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                TextSwitch {
                    id: hideReadSwitch
                    automaticCheck: false
                    //% "Hide read"
                    text: qsTrId("fuoten-hide-read-label")
                    //% "Depending on the context, feeds or folders with zero unread articles or unread articles itself will be hidden."
                    description: qsTrId("fuoten-hide-read-desc")
                    checked: cc.hideRead
                    onClicked: cc.hideRead = !cc.hideRead
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: sortByFolderSwitch.implicitHeight
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                visible: cc.contextType === FuotenApp.StartPage && config.mainViewType === Fuoten.Feed

                TextSwitch {
                    id: sortByFolderSwitch
                    automaticCheck: false
                    //% "Sort by folder"
                    text: qsTrId("fuoten-sortbyfolder-label")
                    //% "Sort the feeds in the list by their folder."
                    description: qsTrId("fuoten-sortbyfolder-desc")
                    checked: cc.showFolderSections
                    onClicked: cc.showFolderSections = !cc.showFolderSections
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: respPinned.implicitHeight
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                visible: cc.contextType === FuotenApp.Feeds || (config.mainViewType === Fuoten.Feed && cc.contextType === FuotenApp.StartPage)

                TextSwitch {
                    id: respPinned
                    automaticCheck: false
                    //% "Respect pinned"
                    text: qsTrId("fuoten-respect-pinned-label")
                    //% "Use the pinned status of feeds to show pinned feeds before not pinned feeds."
                    description: qsTrId("fuoten-respect-pinned-desc")
                    checked: cc.respectPinned
                    onClicked: cc.respectPinned = !cc.respectPinned
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: excerptSwitch.implicitHeight
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                visible: cc.contextType > FuotenApp.Feeds

                TextSwitch {
                    id: excerptSwitch
                    automaticCheck: false
                    //% "Show excerpt"
                    text: qsTrId("fuoten-show-excerpt-label")
                    //% "Shows some lines of the article content in the article list."
                    description: qsTrId("fuoten-show-excerpt-desc")
                    checked: cc.showExcerpt
                    onClicked: cc.showExcerpt = !cc.showExcerpt
                }
            }

            SectionHeader {
                //% "Article view"
                text: qsTrId("fuoten-config-section-article-view")
                Layout.columnSpan: contextConfigGrid.columns
                Layout.preferredWidth: contextConfigGrid.width - Theme.horizontalPageMargin
                visible: cc.contextType > FuotenApp.Feeds
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: openInChoser.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.columnSpan: contextConfigGrid.columns
                visible: cc.contextType > FuotenApp.Feeds

                ComboBox {
                    id: openInChoser
                    //% "Open articles"
                    label: qsTrId("fuoten-open-in-label")
                    //% "Choose the way you want to open articles."
                    description: qsTrId("fuoten-open-in-desc")
                    menu: ContextMenu {
                        MenuItem {
                            //% "Internal"
                            text: qsTrId("fuoten-open-article-internal")
                            readonly property int value: FuotenApp.OpenInternal
                        }
                        MenuItem {
                            //% "Internal web view"
                            text: qsTrId("fuoten-open-article-webview")
                            readonly property int value: FuotenApp.OpenWebView
                        }
                        MenuItem {
                            //% "External browser"
                            text: qsTrId("fuoten-open-article-external")
                            readonly property int value: FuotenApp.OpenExternal
                        }
                    }

                    onCurrentIndexChanged: if (currentItem) { cc.openArticles = currentItem.value }
                    currentIndex: cc.openArticles
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: userAgentChoser.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.columnSpan: contextConfigGrid.columns
                visible: (cc.contextType > FuotenApp.Feeds) && (cc.openArticles === FuotenApp.OpenWebView)

                ComboBox {
                    id: userAgentChoser
                    //% "User agent"
                    label: qsTrId("fuoten-user-agent")
                    //% "The user agent used for the internal web view."
                    description: qsTrId("fuoten-user-agent-desc")
                    menu: ContextMenu {
                        Repeater {
                            model: UserAgentModel { id: userAgentModel }
                            MenuItem { text: model.title; readonly property string value: model.agent }
                        }
                    }
                    currentIndex: cc.userAgentIdx
                    onCurrentIndexChanged: if (currentItem) { cc.userAgent = currentItem.value; cc.userAgentIdx = currentIndex }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: minimumFontSizeSlider.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.columnSpan: contextConfigGrid.columns
                visible: (cc.contextType > FuotenApp.Feeds) && (cc.openArticles === FuotenApp.OpenWebView)

                FontSizeSlider {
                    id: minimumFontSizeSlider
                    width: parent.width
                    //% "Minimum font size for web view"
                    label: qsTrId("fuoten-contextconfig-minimum-font-size")
                    value: (cc.minimumFontSize > 0) ? cc.minimumFontSize : Theme.fontSizeExtraSmall
                    onChangeTriggered: cc.minimumFontSize = Math.floor(minimumFontSizeSlider.sliderValue)
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: defaultFontSizeSlider.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.columnSpan: contextConfigGrid.columns
                visible: (cc.contextType > FuotenApp.Feeds) && (cc.openArticles === FuotenApp.OpenWebView)

                FontSizeSlider {
                    id: defaultFontSizeSlider
                    width: parent.width
                    //% "Default font size for web view"
                    label: qsTrId("fuoten-contextconfig-default-font-size")
                    value: (cc.defaultFontSize > 0) ? cc.defaultFontSize : Theme.fontSizeSmall
                    onChangeTriggered: cc.defaultFontSize = Math.floor(defaultFontSizeSlider.sliderValue)
                }
            }

            SectionHeader {
                //% "Maintenance"
                text: qsTrId("fuoten-config-section-maintenance")
                Layout.columnSpan: contextConfigGrid.columns
                Layout.preferredWidth: contextConfigGrid.width - Theme.horizontalPageMargin
                visible: cc.contextType === FuotenApp.FeedItems
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: deletionStrategyChoser.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                visible: cc.contextType === FuotenApp.FeedItems

                ComboBox {
                    id: deletionStrategyChoser
                    //% "Articles cleanup"
                    label: qsTrId("fuoten-articles-cleanup")
                    description: currentIndex === 0
                                    //% "Articles will never be cleaned up."
                                 ? qsTrId("fuoten-articles-no-cleanup-desc")
                                 : currentIndex === 1
                                     //% "Articles will be deleted if they have been published more than %n days ago. Set the number of days in the next input field."
                                   ? qsTrId("fuoten-articles-cleanup-time-desc", cc.deletionValue)
                                     //% "Articles will be deleted if there are more than %n articles in the local storage. Set the number in the next input field."
                                   : qsTrId("fuoten-articles-cleanup-count-desc", cc.deletionValue)
                    menu: ContextMenu {
                        MenuItem {
                            //% "No cleanup"
                            text: qsTrId("fuoten-articles-no-cleanup")
                            readonly property int value: Fuoten.NoItemDeletion
                        }
                        MenuItem {
                            //% "By time"
                            text: qsTrId("fuoten-articles-cleanup-time")
                            readonly property int value: Fuoten.DeleteItemsByTime
                        }
                        MenuItem {
                            //% "By count"
                            text: qsTrId("fuoten-articles-cleanup-count")
                            readonly property int value: Fuoten.DeleteItemsByCount
                        }
                    }

                    onCurrentIndexChanged: if (currentItem) { cc.deletionStrategy = currentItem.value }
                    currentIndex: cc.deletionStrategy
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: deletionValueField.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                visible: cc.contextType === FuotenApp.FeedItems && cc.deletionStrategy !== Fuoten.NoItemDeletion

                TextField {
                    id: deletionValueField
                    width: parent.width
                    placeholderText: cc.deletionStrategy === Fuoten.DeleteItemsByTime
                                       //% "Days old"
                                     ? qsTrId("fuoten-articles-cleanup-days")
                                       //% "Count to keep"
                                     : qsTrId("fuoten-articles-cleanup-number")
                    label: placeholderText
                    validator: IntValidator { bottom: 0; top: 65535 }
                    text: cc.deletionValue
                    onTextChanged: if (acceptableInput) cc.deletionValue = parseInt(text)
                    inputMethodHints: Qt.ImhDigitsOnly
                }
            }
        }
    }

}

