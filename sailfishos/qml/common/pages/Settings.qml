/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/qml/phone/pages/MainPage.qml
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
    id: settingsPage

    SilicaFlickable {
        id: settingsFlick
        anchors.fill: parent
        contentHeight: settingsGrid.height

        VerticalScrollDecorator { flickable: settingsFlick; page: settingsPage }

        GridLayout {
            id: settingsGrid
            anchors { left: parent.left; right: parent.right; }

            columnSpacing: 0
            rowSpacing: Theme.paddingSmall

            columns: Screen.sizeCategory < Screen.Large ? (settingsPage.isLandscape ? 2 : 1) : (settingsPage.isLandscape ? 4 : 2)

            PageHeader {
                //% "Settings"
                title: qsTrId("id-settings")
                page: settingsPage
                Layout.columnSpan: settingsGrid.columns
                Layout.fillWidth: true
            }

            SectionHeader {
                //% "User account"
                text: qsTrId("id-user-account")
                Layout.columnSpan: settingsGrid.columns
                Layout.preferredWidth: settingsGrid.width - Theme.horizontalPageMargin
            }

            AccountItem {
                id: accountArea
                Layout.fillWidth: true
                Layout.columnSpan: settingsGrid.columns
                Layout.preferredHeight: contentHeight
            }


            SectionHeader {
                //% "Appearance"
                text: qsTrId("id-appearance")
                Layout.columnSpan: settingsGrid.columns
                Layout.preferredWidth: settingsGrid.width - Theme.horizontalPageMargin
            }


            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: langChoser.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                ComboBox {
                    id: langChoser
                    //% "Language"
                    label: qsTrId("id-lang-choser-label")
                    //% "To change the language, you have to restart the application."
                    description: qsTrId("id-lang-choser-desc")
                    menu: ContextMenu {
                        Repeater {
                            model: LanguageModel { id: langModel }
                            MenuItem { text: model.name; readonly property string value: model.code }
                        }
                    }
                    onCurrentIndexChanged: if (currentItem) { config.language = currentItem.value }
                    currentIndex: langModel.findIndex(config.language)
                }
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
                    onCurrentIndexChanged: if (currentItem) { config.mainViewType = currentItem.value }
                    currentIndex: config.mainViewType
                }
            }

        }
    }
}

