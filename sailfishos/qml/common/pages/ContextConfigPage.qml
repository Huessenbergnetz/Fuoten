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
    id: contextConfigPage

    property ContextConfig cc: null
    property alias name: contextHeader.description

    SilicaFlickable {
        id: contextConfigFlick
        anchors.fill: parent
        contentHeight: contextConfigGrid.height

        PullDownMenu {
            flickable: contextConfigFlick
            MenuItem {
                //: Menu entry and page header to create a new folder
                //% "Create folder"
                text: qsTrId("fuoten-create-folder")
                onClicked: pageStack.push(Qt.resolvedUrl("../dialogs/CreateFolderDialog.qml"))
            }
            MenuItem {
                //: Menu entry and page header to add a new feed
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
                title: cc.contextType === FuotenApp.StartPage
                       //: Page header of the context config page if the current context is the main view
                       //% "Main view settings"
                       ? qsTrId("fuoten-mainview-settings")
                       : cc.contextType === FuotenApp.Feeds
                         //: Page header on a context config page if the current context is a folder view showing a list of feeds
                         //% "Folder settings"
                         ? qsTrId("fuoten-folder-settings")
                         //: Page header on a context config page if the current context is a list of articles
                         //% "Articles list settings"
                         : qsTrId("fuoten-articles-list-settings")
                page: contextConfigPage
                Layout.columnSpan: contextConfigGrid.columns
                Layout.fillWidth: true
            }

            SectionHeader {
                //: Section header on a context config page
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
                    //: Label for a combo box to select content sorting. The visible value after the label will be something like time, title, etc.
                    //% "Sort by"
                    label: qsTrId("fuoten-sort-by-label")
                    //: Description for a combo box to select content sorting
                    //% "Choose the role you want to sort the content by."
                    description: qsTrId("fuoten-sort-by-desc")
                    menu: ContextMenu {
                        MenuItem {
                            //: Selectable entry in the sort by combo box
                            //% "Database ID"
                            text: qsTrId("fuoten-sort-database-id")
                            readonly property int value: Fuoten.ID
                        }
                        MenuItem {
                            //: Selectable entry in the sort by combo box
                            //% "Title"
                            text: qsTrId("fuoten-sort-title")
                            readonly property int value: Fuoten.Name
                        }
                        MenuItem {
                            //: Selectable entry in the sort by combo box
                            //% "Time"
                            text: qsTrId("fuoten-sort-time")
                            readonly property int value: Fuoten.Time
                            visible: cc.contextType > FuotenApp.Feeds
                        }
                        MenuItem {
                            //: Selectable entry in the sort by combo box
                            //% "Unread count"
                            text: qsTrId("fuoten-sort-unread-count")
                            readonly property int value: Fuoten.UnreadCount
                        }
                        MenuItem {
                            //: Selectable entry in the sort by combo box
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
                    //: Label for a context config switch to enable descending sort order of feeds, folders or articles
                    //% "Sort descending"
                    text: qsTrId("fuoten-sort-descending-label")
                    //: Description for a context config switch to enable descending sort order of feeds, folders or articles
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
                    //: Label for a switch to hide read feeds, folders or articles, depending on the current context
                    //% "Hide read"
                    text: qsTrId("fuoten-hide-read-label")
                    //: Description for a switch to hide read feeds, folders or articles, depending on the current context
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
                    //: Label for a switch to sort feeds by folder name
                    //% "Sort by folder"
                    text: qsTrId("fuoten-sortbyfolder-label")
                    //: Description for a switch to sort feeds by folder name
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
                    //: Label for a switch to respect the pinned order of feeds
                    //% "Respect pinned"
                    text: qsTrId("fuoten-respect-pinned-label")
                    //: Description for a switch to respect the pinned order of feeds
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
                    //: Label for a switch to show excerts of articles in the article list view
                    //% "Show excerpt"
                    text: qsTrId("fuoten-show-excerpt-label")
                    //: Description for a switch to show excerts of articles in the article list view
                    //% "Shows some lines of the article content in the article list."
                    description: qsTrId("fuoten-show-excerpt-desc")
                    checked: cc.showExcerpt
                    onClicked: cc.showExcerpt = !cc.showExcerpt
                }
            }

            SectionHeader {
                //: Section header on a context config page
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
                    //: Label for a combo box on the context config to select the way article content will be opened. Value will be something like internal, external browser, etc.
                    //% "Open articles"
                    label: qsTrId("fuoten-open-in-label")
                    //: Description for a combo box on the context config to select the way article content will be opened
                    //% "Choose the way you want to open articles."
                    description: qsTrId("fuoten-open-in-desc")
                    menu: ContextMenu {
                        MenuItem {
                            //: Selectabe entry in the combo box to choose the way articles are opened
                            //% "Internal"
                            text: qsTrId("fuoten-open-article-internal")
                            readonly property int value: FuotenApp.OpenInternal
                        }
                        MenuItem {
                            //: Selectabe entry in the combo box to choose the way articles are opened
                            //% "Internal web view"
                            text: qsTrId("fuoten-open-article-webview")
                            readonly property int value: FuotenApp.OpenWebView
                        }
                        MenuItem {
                            //: Selectabe entry in the combo box to choose the way articles are opened
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
                Layout.preferredHeight: webViewJsSupportPicker.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.columnSpan: contextConfigGrid.columns
                visible: cc.contextType > FuotenApp.Feeds && cc.openArticles === FuotenApp.OpenWebView

                ComboBox {
                    id: webViewJsSupportPicker
                    //: Label for a combo box on the context config page to select the support for JavaScript for the internal WebView.
                    //% "JavaScript support"
                    label: qsTrId("fuoten-webview-jssupport-label")
                    //: Description for a combo box on the context config page to select support for JavaScript for the internal WebView.
                    //% "Whether JavaScript on the page will be automatically executed in the internal web view. Most modern web pages will not work with JavaScript disabled."
                    description: qsTrId("fuoten-webview-jssupport-desc")
                    menu: ContextMenu {
                        MenuItem {
                            //: Selectable entry in the combo box to choose JavaScript support,
                            //: %1 will be replaced by the default value
                            //% "Default (%1)"
                            text: qsTrId("fuoten-jssupport-default").arg(config.jsSupport == FuotenApp.JsEnabled
                                                                         //: Selectable entry in the combo box to choose JavaScript support
                                                                         //% "Enabled"
                                                                         ? qsTrId("fuoten-jssupport-enabled")
                                                                         //: Selectable entry in the combo box to choose JavaScript support
                                                                         //% "Disabled"
                                                                         : qsTrId("fuoten-jssupport-disabled"))
                            readonly property int value: FuotenApp.JsDefault
                        }
                        MenuItem {
                            text: qsTrId("fuoten-jssupport-enabled")
                            readonly property int value: FuotenApp.JsEnabled
                        }
                        MenuItem {
                            text: qsTrId("fuoten-jssupport-disabled")
                            readonly property int value: FuotenApp.JsDisabled
                        }
                    }
                    onCurrentIndexChanged: if (currentItem) { cc.jsSupport = currentItem.value }
                    currentIndex: {
                        var menuEntries = webViewJsSupportPicker.menu.children
                        var length = menuEntries.length
                        for (var i = 0; i < length; i++) {
                            if (menuEntries[i].value == cc.jsSupport) {
                                return i;
                            }
                        }
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: webViewCookieBehaviorPicker.height
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.columnSpan: contextConfigGrid.columns
                visible: cc.contextType > FuotenApp.Feeds && cc.openArticles !== FuotenApp.OpenExternal

                ComboBox {
                    id: webViewCookieBehaviorPicker
                    //: Label for a combo box on the context config page to select cookie behavior
                    //% "Cookies"
                    label: qsTrId("fuoten-webview-cookiebehavior-label")
                    //: Description for a combo box on the context config page to select cookie behavior
                    //% "How cookies should be handled."
                    description: qsTrId("fuoten-webview-cookiebehavior-desc")
                    menu: ContextMenu {
                        MenuItem {
                            id: cookieBehaviorMenuItem

                            readonly property int value: FuotenApp.CookiesDefault

                            Component.onCompleted: {
                                var _defVal;
                                switch(config.cookieBehavior) {
                                case FuotenApp.CookiesAcceptAll:
                                    _defVal = qsTrId("fuoten-cookiebehavior-accept-all")
                                    break
                                case FuotenApp.CookiesBlockAll:
                                    _defVal = qsTrId("fuoten-cookiebehavior-block-all")
                                    break
                                case FuotenApp.CookiesBlockThirdParty:
                                    _defVal = qsTrId("fuoten-cookiebehavior-block-3rd")
                                    break
                                }
                                //: Selectable entry in the combo box to choose cookie behavior
                                //: %1 will be replaced by the default value
                                //% "Default (%1)"
                                cookieBehaviorMenuItem.text = qsTrId("fuoten-cookiebehavior-default").arg(_defVal)
                            }
                        }
                        MenuItem {
                            //: Selectable entry in the combo box to choose cookie behavior
                            //% "Accept all"
                            text: qsTrId("fuoten-cookiebehavior-accept-all")
                            readonly property int value: FuotenApp.CookiesAcceptAll
                        }
                        MenuItem {
                            //: Selectable entry in the combo box to choose cookie behavior
                            //% "Block all"
                            text: qsTrId("fuoten-cookiebehavior-block-all")
                            readonly property int value: FuotenApp.CookiesBlockAll
                        }
                        MenuItem {
                            //: Selectable entry in the combo box to choose cookie behavior
                            //% "Block 3rd party"
                            text: qsTrId("fuoten-cookiebehavior-block-3rd")
                            readonly property int value: FuotenApp.CookiesBlockThirdParty
                        }
                    }
                    onCurrentIndexChanged: if (currentItem) { cc.cookieBehavior = currentItem.value }
                    currentIndex: {
                        var menuEntries = webViewCookieBehaviorPicker.menu.children
                        var length = menuEntries.length
                        for (var i = 0; i < length; i++) {
                            if (menuEntries[i].value == cc.cookieBehavior) {
                                return i;
                            }
                        }
                    }
                }
            }

//            Item {
//                Layout.fillWidth: true
//                Layout.preferredHeight: userAgentChoser.height
//                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
//                Layout.columnSpan: contextConfigGrid.columns
//                visible: (cc.contextType > FuotenApp.Feeds) && (cc.openArticles !== FuotenApp.OpenExternal)

//                ComboBox {
//                    id: userAgentChoser
//                    //: Label for a combo box on a context config page to set the user agent used in the internal web view
//                    //% "User agent"
//                    label: qsTrId("fuoten-user-agent")
//                    //: Description for a combo box on a context config page to set the user agent used in the internal web view
//                    //% "The user agent used for the internal web view."
//                    description: qsTrId("fuoten-user-agent-desc")
//                    menu: ContextMenu {
//                        Repeater {
//                            model: UserAgentModel { id: userAgentModel }
//                            MenuItem { text: model.title; readonly property string value: model.agent }
//                        }
//                    }
//                    currentIndex: cc.userAgentIdx
//                    onCurrentIndexChanged: if (currentItem) { cc.userAgent = currentItem.value; cc.userAgentIdx = currentIndex }
//                }
//            }

//            Item {
//                Layout.fillWidth: true
//                Layout.preferredHeight: minimumFontSizeSlider.height
//                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
//                Layout.columnSpan: contextConfigGrid.columns
//                visible: (cc.contextType > FuotenApp.Feeds) && (cc.openArticles !== FuotenApp.OpenExternal)

//                FontSizeSlider {
//                    id: minimumFontSizeSlider
//                    width: parent.width
//                    //: Label for a slider on a context config page to select the minimum font size for the internal article web view
//                    //% "Minimum font size for web view"
//                    label: qsTrId("fuoten-contextconfig-minimum-font-size")
//                    value: (cc.minimumFontSize > 0) ? cc.minimumFontSize : Theme.fontSizeExtraSmall
//                    onChangeTriggered: cc.minimumFontSize = Math.floor(minimumFontSizeSlider.sliderValue)
//                }
//            }

//            Item {
//                Layout.fillWidth: true
//                Layout.preferredHeight: defaultFontSizeSlider.height
//                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
//                Layout.columnSpan: contextConfigGrid.columns
//                visible: (cc.contextType > FuotenApp.Feeds) && (cc.openArticles !== FuotenApp.OpenExternal)

//                FontSizeSlider {
//                    id: defaultFontSizeSlider
//                    width: parent.width
//                    //: Label for a slider on a context config page to select the default font size for the internal article web view
//                    //% "Default font size for web view"
//                    label: qsTrId("fuoten-contextconfig-default-font-size")
//                    value: (cc.defaultFontSize > 0) ? cc.defaultFontSize : Theme.fontSizeSmall
//                    onChangeTriggered: cc.defaultFontSize = Math.floor(defaultFontSizeSlider.sliderValue)
//                }
//            }

            SectionHeader {
                //: Section header on a context config page
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
                    //: Label for a combo box on a context config page to select the article clean up strategy
                    //% "Articles cleanup"
                    label: qsTrId("fuoten-articles-cleanup")
                    description: currentIndex === 0
                                    //: Description for a combo box on a context config page to select article clean up strategy if cleanup has been disabled
                                    //% "Articles will never be cleaned up."
                                 ? qsTrId("fuoten-articles-no-cleanup-desc")
                                 : currentIndex === 1
                                     //: Description for a combo box on a context config page to select article clean up strategy if cleanup will be done based on the article age. %n will contain the number of days after which articles are deleted from the local storage
                                     //% "Articles will be deleted if they have been published more than %n days ago. Set the number of days in the next input field."
                                   ? qsTrId("fuoten-articles-cleanup-time-desc", cc.deletionValue)
                                     //: Description for a combo box on a context config page to select article clean up strategy if cleanup will be done based on the article count. %n will contain the number of articles that will be kept in the local storage
                                     //% "Articles will be deleted if there are more than %n articles in the local storage. Set the number in the next input field."
                                   : qsTrId("fuoten-articles-cleanup-count-desc", cc.deletionValue)
                    menu: ContextMenu {
                        MenuItem {
                            //: Selectable value in the combo box on the context config page to choose the article clean up strategy
                            //% "No cleanup"
                            text: qsTrId("fuoten-articles-no-cleanup")
                            readonly property int value: Fuoten.NoItemDeletion
                        }
                        MenuItem {
                            //: Selectable value in the combo box on the context config page to choose the article clean up strategy
                            //% "By time"
                            text: qsTrId("fuoten-articles-cleanup-time")
                            readonly property int value: Fuoten.DeleteItemsByTime
                        }
                        MenuItem {
                            //: Selectable value in the combo box on the context config page to choose the article clean up strategy
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
                                       //: Label and placeholder text for a text input on the context config page to set the age in days after which articles will be removed from the local storage
                                       //% "Days old"
                                     ? qsTrId("fuoten-articles-cleanup-days")
                                       //: Label and placeholder text for a text input on the context config page to set the number of articles that will be kept in local storage
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

