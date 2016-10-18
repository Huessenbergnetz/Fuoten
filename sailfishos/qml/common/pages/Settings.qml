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
import harbour.fuoten.generic 1.0
import "../parts"

Page {
    id: settingsPage

    AccountValidator {
        id: accValidator
        configuration: config
    }

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

            BackgroundItem {
                id: accountArea
                Layout.fillWidth: true
                Layout.columnSpan: settingsGrid.columns
                Layout.preferredHeight: contentHeight
                contentHeight: Theme.itemSizeExtraLarge

                enabled: !accValidator.inOperation

                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/AccountSetup.qml"))
                    dialog.accepted.connect(function() {
                        accValidator.start()
                    })
                }

                BusyIndicator {
                    visible: accValidator.inOperation
                    running: accValidator.inOperation
                    size: BusyIndicatorSize.Medium
                    anchors.centerIn: parent
                }

                ErrorItem {
                    anchors {left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
                    visible: !accValidator.inOperation && accValidator.error
                    error: accValidator.error
                    highlighted: accountArea.highlighted
                }

                Text {
                    id: invalidAccountText
                    anchors {left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
                    font.pixelSize: Theme.fontSizeSmall
                    color: accountArea.highlighted ? Theme.highlightColor : Theme.primaryColor
                    //% "You account is not or improperly configured. Click here to configure your account."
                    text: qsTrId("id-settings-account-invalid")
                    wrapMode: Text.WordWrap
                    visible: !config.isAccountValid && !accValidator.inOperation && !accValidator.error
                }

                RowLayout {
                    id: accRow
                    anchors {left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
                    spacing: Theme.paddingSmall
                    visible: config.isAccountValid && !accValidator.inOperation && !accValidator.error

                    Image {
                        id: avatarImage
                        source: config.avatar
                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                        Layout.preferredWidth: Theme.iconSizeLarge
                        fillMode: Image.PreserveAspectFit
                    }

                    Column {
                        Layout.fillWidth: true
                        spacing: Theme.paddingSmall

                        Row {
                            width: parent.width
                            spacing: Theme.paddingSmall
                            Text {
                                id: displayNameString
                                text: config.displayName
                                font.pixelSize: Theme.fontSizeSmall
                                color: accountArea.highlighted ? Theme.highlightColor : Theme.primaryColor
                                textFormat: Text.PlainText
                                maximumLineCount: 1
                            }
                            Label {
                                id: usernameString
                                text: config.displayName ? String("(%1)").arg(config.username) : config.username
                                font.pixelSize: Theme.fontSizeSmall
                                color: accountArea.highlighted ? Theme.highlightColor : Theme.primaryColor
                                textFormat: Text.PlainText
                                width: config.displayName ? parent.width - displayNameString.width - parent.spacing : parent.width
                                truncationMode: TruncationMode.Fade
                                maximumLineCount: 1
                            }
                        }
                        Label {
                            width: parent.width
                            font.pixelSize: Theme.fontSizeSmall
                            color: accountArea.highlighted ? Theme.highlightColor : Theme.primaryColor
                            text: String("%1%2").arg(config.host).arg(config.installPath)
                            truncationMode: TruncationMode.Fade
                            maximumLineCount: 1
                        }
                        RowLayout {
                            id: statusRow
                            spacing: Theme.paddingSmall

                            Item {
                                Layout.fillWidth: true
                                Layout.preferredWidth: versionString.implicitWidth
                                height: versionString.height
                                Text {
                                    id: versionString
                                    font.pixelSize: Theme.fontSizeSmall
                                    color: accountArea.highlighted ? Theme.highlightColor : Theme.primaryColor
                                    //: %1 is the News App version number
                                    //% "Version %1"
                                    text: qsTrId("id-settings-version").arg(config.serverVersion)
                                    textFormat: Text.PlainText
                                    width: parent.width
                                }
                            }
                            Image {
                                id: statusIcon
                                property color color: accountArea.highlighted ? Theme.highlightColor : Theme.primaryColor
                                source: config.useSSL && !config.ignoreSSLErrors ? "image://theme/icon-s-installed?" + color : "image://theme/icon-s-high-importance?" + color
                                Layout.preferredWidth: Theme.iconSizeSmall
                                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                                fillMode: Image.PreserveAspectFit
                            }
                            Item {
                                Layout.preferredWidth: accRow.width - versionString.implicitWidth - avatarImage.width - statusIcon.implicitWidth - accRow.spacing - (2* statusRow.spacing)
                                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                                height: statusString.height
                                Label {
                                    id: statusString
                                    width: parent.width
                                    font.pixelSize: Theme.fontSizeSmall
                                    color: accountArea.highlighted ? Theme.highlightColor : Theme.primaryColor
                                    text: config.useSSL && !config.ignoreSSLErrors
                                          //% "HTTPS enabled"
                                          ? qsTrId("id-settings-ssl-enabled")
                                          : config.useSSL && config.ignoreSSLErrors
                                            //% "Ignoring errors"
                                            ? qsTrId("id-settings-ignore-ssl-errors")
                                              //% "HTTPS disabled"
                                            : qsTrId("id-settings-ssl-disabled")
                                    truncationMode: TruncationMode.Fade
                                    textFormat: Text.PlainText
                                }
                            }
                        }
                    }
                }
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

