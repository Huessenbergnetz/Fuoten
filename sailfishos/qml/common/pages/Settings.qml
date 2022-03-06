/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
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

        PullDownMenu {
            MenuItem {
                text: qsTrId("fuoten-deleteaccount-title")
                onClicked: pageStack.push(Qt.resolvedUrl("../wizard/DeleteAccount.qml"))
            }
        }

        VerticalScrollDecorator { flickable: settingsFlick; page: settingsPage }

        GridLayout {
            id: settingsGrid
            anchors { left: parent.left; right: parent.right; }

            columnSpacing: 0
            rowSpacing: Theme.paddingSmall

            columns: Screen.sizeCategory < Screen.Large ? (settingsPage.isLandscape ? 2 : 1) : (settingsPage.isLandscape ? 4 : 2)

            PageHeader {
                //: Menu entry and page header
                //% "Settings"
                title: qsTrId("id-settings")
                page: settingsPage
                Layout.columnSpan: settingsGrid.columns
                Layout.fillWidth: true
            }

            SectionHeader {
                //: Used as dialog and section header for displaying and configuring the user account
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
                Component.onCompleted: accountArea.check()
            }


            SectionHeader {
                //: Section header on the settings page
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
                    //: Label for a combo box on the settings page to choose the application language
                    //% "Language"
                    label: qsTrId("id-lang-choser-label")
                    //: Description for a combo box on the settings page to choose the application language
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
                    //: Label for a combo box on the settings page to select the content of the first applicatin page
                    //% "Main view content"
                    label: qsTrId("id-main-content-choser-label")
                    //: Description for a combo box on the settings page to select the content of the first applicatin page
                    //% "Choose the content to display on the first application page."
                    description: qsTrId("id-main-content-choser-desc")
                    menu: ContextMenu {
                        MenuItem {
                            //: Selectable value in the combo box on the settings page to select the main view content
                            //% "Feeds"
                            text: qsTrId("fuoten-main-content-feeds")
                            readonly property int value: Fuoten.Feed
                        }
                        MenuItem {
                            //: Selectable value in the combo box on the settings page to select the main view content
                            //% "Folders"
                            text: qsTrId("fuoten-main-content-folders")
                            readonly property int value: Fuoten.Folder
                        }
                    }
                    onCurrentIndexChanged: if (currentItem) { config.mainViewType = currentItem.value }
                    currentIndex: config.mainViewType
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: articleFontSizeSlider.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                FontSizeSlider {
                    id: articleFontSizeSlider
                    width: parent.width
                    //: Label for a slider on the settings page to select the font size for the internal view
                    //% "Font size used for internal article view"
                    label: qsTrId("fuoten-settings-article-font-size")
                    value: (config.articleFontSize > 0) ? config.articleFontSize : Theme.fontSizeSmall
                    onChangeTriggered: config.articleFontSize = Math.floor(articleFontSizeSlider.sliderValue)
                }
            }

            SectionHeader {
                //: section header on the settings page
                //% "Behavior"
                text: qsTrId("fuoten-settings-behavior-section")
                Layout.columnSpan: settingsGrid.columns
                Layout.preferredWidth: settingsGrid.width - Theme.horizontalPageMargin
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: intervalChoser.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                ComboBox {
                    id: intervalChoser
                    //: Label for a combo box on the the settings page to select the automatic update interval
                    //% "Update interval"
                    label: qsTrId("fuoten-settings-update-interval")
                    //: Description for a combo box on the the settings page to select the automatic update interval
                    //% "Fuoten can perform pseudo backgrund updates by simply reacting to status changes of the user interface. Be aware that this approach will not perform real background updates while your device is sleeping."
                    description: qsTrId("fuoten-settings-update-interval-sailfish-desc")
                    menu: ContextMenu {
                        Repeater {
                            model: UpdateIntervalModel { id: updateIntModel }
                            MenuItem { text: model.name; readonly property int value: model.interval }
                        }
                    }
                    onCurrentIndexChanged: {
                        if (currentItem) {
                            config.updateInterval = currentItem.value
                            if (!synchronizer.inOperation) {
                                config.checkUpdate()
                            }
                        }
                    }
                    currentIndex: updateIntModel.findIndex(config.updateInterval)
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: updateOnWlanOnlySwitch.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                TextSwitch {
                    id: updateOnWlanOnlySwitch
                    enabled: config.updateInterval > 0
                    automaticCheck: false
                    checked: config.wlanOnlyUpdate
                    //: Label for a switch on the settings page
                    //% "Automatic synchronization only on WLAN"
                    text: qsTrId("fuoten-settings-wlan-only-updates")
                    //: Description for a switch on the settings page
                    //% "If enabled, automatic pseudo background updates will only be performed if the device is connected to a WLAN."
                    description: qsTrId("fuoten-settings-wlan-only-updates-desc")
                    onClicked: {
                        config.wlanOnlyUpdate = !config.wlanOnlyUpdate
                        if (!synchronizer.inOperation) {
                            config.checkUpdate()
                        }
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: enableNotificationsSwitch.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                TextSwitch {
                    id: enableNotificationsSwitch
                    automaticCheck: false
                    checked: config.notificationsEnabled
                    //: Label for a switch on the settings page
                    //% "Enable notifications"
                    text: qsTrId("fuoten-settings-enable-notifications")
                    //: Description for a switch on the settings page
                    //% "If enabled, Fuoten will show notifications when specific operations succeed or if errors occure. Most of this notifications will only be generated if the application is not the active one."
                    description: qsTrId("fuoten-settings-enable-notifications-desc")
                    onClicked: {
                        config.notificationsEnabled = !config.notificationsEnabled
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: enablePushUpOnArticle.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                TextSwitch {
                    id: enablePushUpOnArticle
                    automaticCheck: false
                    checked: config.pushUpOnArticle
                    //: Label for a switch on the settings page
                    //% "Show push up menu on article page"
                    text: qsTrId("fuoten-settings-pushuponarticle")
                    //: Description for a switch on the settings page
                    //% "If enabled, article pages will have a push up menu on the end with the same content as the pull down menu for faster access after an article has been read."
                    description: qsTrId("fuoten-settings-pushuponarticle-desc")
                    onClicked: {
                        config.pushUpOnArticle = !config.pushUpOnArticle
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: enableNavBackAfterMark.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                TextSwitch {
                    id: enableNavBackAfterMark
                    automaticCheck: false
                    checked: config.navBackAfterMark
                    //: Label for a switch on the settings page
                    //% "Navigate back after marking as read"
                    text: qsTrId("fuoten-settings-navbackaftermark")
                    //: Description for a switch on the settings page
                    //% "If activated, after marking a complete feed or folder as read, the application automatically switches back to the parent page."
                    description: qsTrId("fuoten-settings-navbackaftermark-desc")
                    onClicked: {
                        config.navBackAfterMark = !config.navBackAfterMark
                    }
                }
            }

            SectionHeader {
                //: section header on the settings page
                //% "Default settings"
                text: qsTrId("fuoten-settings-defaults-section")
                Layout.columnSpan: settingsGrid.columns
                Layout.preferredWidth: settingsGrid.width - Theme.horizontalPageMargin
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: defaultJsSupportPicker.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                ComboBox {
                    id: defaultJsSupportPicker
                    label: qsTrId("fuoten-webview-jssupport-label")
                    description: qsTrId("fuoten-webview-jssupport-desc")
                    menu: ContextMenu {
                        MenuItem {
                            text: qsTrId("fuoten-jssupport-enabled")
                            readonly property int value: FuotenApp.JsEnabled
                        }
                        MenuItem {
                            text: qsTrId("fuoten-jssupport-disabled")
                            readonly property int value: FuotenApp.JsDisabled
                        }
                    }
                    onCurrentIndexChanged: if (currentItem) { config.jsSupport = currentItem.value }
                    currentIndex: {
                        var menuEntries = defaultJsSupportPicker.menu.children
                        var length = menuEntries.length
                        for (var i = 0; i < length; i++) {
                            if (menuEntries[i].value == config.jsSupport) {
                                return i;
                            }
                        }
                    }
                }
            }
        }
    }
}

