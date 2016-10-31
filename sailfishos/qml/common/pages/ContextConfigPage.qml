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
                       : cc.contextType === FuotenApp.Items
                         //% "Feed settings"
                         ? qsTrId("fuoten-feed-settings")
                         //% "Folder settings"
                         : qsTrId("fuoten-folder-settings")
                page: contextConfigPage
                Layout.columnSpan: contextConfigGrid.columns
                Layout.fillWidth: true
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: sortingChoser.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

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
                            visible: cc.contextType === FuotenApp.Items
                        }
                        MenuItem {
                            //% "Unread count"
                            text: qsTrId("fuoten-sort-unread-count")
                            readonly property int value: Fuoten.UnreadCount
                            visible: cc.contextType !== FuotenApp.Items && cc.contextType !== FuotenApp.SingleItem
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
        }
    }

}

