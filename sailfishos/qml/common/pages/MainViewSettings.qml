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
    id: mainViewSettingsPage

    SilicaFlickable {
        id: mainViewSettingsFlick
        anchors.fill: parent
        contentHeight: mainViewSettingsGrid.height

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

        VerticalScrollDecorator { flickable: mainViewSettingsFlick; page: mainViewSettingsPage }

        GridLayout {
            id: mainViewSettingsGrid
            anchors { left: parent.left; right: parent.right }

            columnSpacing: 0
            rowSpacing: Theme.paddingSmall

            columns: Screen.sizeCategory < Screen.Large ? (mainViewSettingsPage.isLandscape ? 2 : 1) : (mainViewSettingsPage.isLandscape ? 4 : 2)

            PageHeader {
                //% "Main view settings"
                title: qsTrId("fuoten-mainview-settings")
                page: settingsPage
                Layout.columnSpan: mainViewSettingsGrid.columns
                Layout.fillWidth: true
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: mainContentChoser.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                ComboBox {
                    id: mainContentChoser
                    //% "Main view content"
                    label: qsTrId("id-main-content-choser-label")
                    //% "Choose the content to display on the first application page."
                    description: qsTrId("id-main-content-choser-desc")
                    menu: ContextMenu {
                        MenuItem {
                            //% "Feeds"
                            text: qsTrId("id-feeds")
                            readonly property int value: Fuoten.Feed
                        }
                        MenuItem {
                            //% "Folders"
                            text: qsTrId("id-folders")
                            readonly property int value: Fuoten.Folder
                        }
                    }
                    onCurrentIndexChanged: {
                        if (currentItem) {
                            config.mainViewType = currentItem.value
                            if (config.mainViewType === Fuoten.Feed && sortingChoser.currentIndex === Fuoten.FeedCount) {
                                sortingChoser.currentIndex = Fuoten.Name
                            }
                        }
                    }
                    currentIndex: config.mainViewType
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: sortingChoser.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                SortingChoser {
                    id: sortingChoser
                    type: config.mainViewType
                    mainView: true
                    onCurrentIndexChanged: if (currentItem) { config.mainViewSorting = currentItem.value }
                    currentIndex: config.mainViewSorting
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: hideReadSwitch.implicitHeight
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                HideReadSwitch {
                    id: hideReadSwitch
                    checked: config.mainViewHideRead
                    onClicked: config.mainViewHideRead = !config.mainViewHideRead
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: sortOrderSwitch.implicitHeight
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                SortOrderSwitch {
                    id: sortOrderSwitch
                    checked: config.mainViewSortOrder === Qt.DescendingOrder
                    onClicked: {
                        if (config.mainViewSortOrder === Qt.DescendingOrder) {
                            config.mainViewSortOrder = Qt.AscendingOrder
                        } else {
                            config.mainViewSortOrder = Qt.DescendingOrder
                        }
                    }
                }
            }
        }
    }

}

