/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
//import harbour.fuoten 1.0
import harbour.fuoten.models 1.0
import harbour.fuoten.items 1.0

Column {
    id: lph

    property alias page: header.page
    property bool folders: true
    property bool folderItems: false
    property bool startPage: !folder && !feed
    property alias searchVisible: searchField.visible
    property alias searchPlaceHolder: searchField.placeholderText
    property alias searchText: searchField.text
    property alias feedListDelegate: feedRepeater.delegate
    property alias title: header.title
    property alias description: header.description
    property Folder folder: null
    property Feed feed: null

    width: parent ? parent.width : Screen.width

    signal allArticlesClicked()
    signal starredItemsClicked()

    PageHeader {
        id: header
        title: folder ? folder.name : feed ? feed.title : "Fuoten"
        description: (folder && folderItems)
                        //% "%n unread article(s)"
                     ? qsTrId("fuoten-unread-articles-with-count", folder.unreadCount)
                     : (folder && !folderItems)
                        //% "%n feed(s)"
                       ? qsTrId("fuoten-feeds-count", folder.feedCount)
                       : feed
                            //% "%n unread article(s)"
                         ? qsTrId("fuoten-unread-articles-with-count", feed.unreadCount)
                         : ""
    }

    SearchField {
        id: searchField
        width: parent.width
        visible: false
        EnterKey.iconSource: "image://theme/icon-m-enter-close"
        EnterKey.onClicked: searchField.focus = false
        onVisibleChanged: {
            if (!visible) {
                text = ""
                searchField.focus = false
            } else {
                searchField.focus = true
            }
        }
    }

    ListItem {
        id: allArticles
        contentHeight: Theme.itemSizeSmall
        visible: !searchField.visible && (startPage || folder) && !folderItems

        property int unreadCount: folder ? folder.unreadCount : localstorage.totalUnread

        onClicked: lph.allArticlesClicked()

        RowLayout {
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
            spacing: Theme.paddingMedium

            Image {
                Layout.preferredWidth: folders ? Theme.iconSizeMedium : Theme.iconSizeSmall
                Layout.preferredHeight: folders ? Theme.iconSizeMedium : Theme.iconSizeSmall
                source: (folders ? "image://fuoten/icon-m-feed?" : "image://fuoten/icon-s-feed?") + (allArticles.highlighted ? (allArticles.unreadCount ? Theme.highlightColor : Theme.secondaryHighlightColor) : (allArticles.unreadCount ? Theme.primaryColor : Theme.secondaryColor))
            }

            Label {
                Layout.fillWidth: true
                //% "All articles"
                text: qsTrId("fuoten-all-articles")
                truncationMode: TruncationMode.Fade
                color: allArticles.highlighted ? (allArticles.unreadCount ? Theme.highlightColor : Theme.secondaryHighlightColor) : (allArticles.unreadCount ? Theme.primaryColor : Theme.secondaryColor)
            }

            Label {
                text: allArticles.unreadCount
                color: allArticles.unreadCount ? Theme.highlightColor : allArticles.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                font.pixelSize: Theme.fontSizeMedium
            }
        }
    }

    ListItem {
        id: starredItems
        contentHeight: Theme.itemSizeSmall
        visible: !searchField.visible && startPage

        onClicked: lph.starredItemsClicked()

        RowLayout {
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
            spacing: Theme.paddingMedium

            Image {
                Layout.preferredWidth: folders ? Theme.iconSizeMedium : Theme.iconSizeSmall
                Layout.preferredHeight: folders ? Theme.iconSizeMedium : Theme.iconSizeSmall
                source: (folders ? "image://theme/icon-m-favorite-selected?" : "image://theme/icon-s-favorite?") + (starredItems.highlighted ? (localstorage.starred ? Theme.highlightColor : Theme.secondaryHighlightColor) : (localstorage.starred ? Theme.primaryColor : Theme.secondaryColor))
            }

            Label {
                Layout.fillWidth: true
                //% "Starred articles"
                text: qsTrId("fuoten-starred-articles")
                truncationMode: TruncationMode.Fade
                color: starredItems.highlighted ? (localstorage.starred ? Theme.highlightColor : Theme.secondaryHighlightColor) : (localstorage.starred ? Theme.primaryColor : Theme.secondaryColor)
            }

            Label {
                text: localstorage.starred
                color: localstorage.starred ? Theme.highlightColor : starredItems.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                font.pixelSize: Theme.fontSizeMedium
            }
        }
    }

    Repeater {
        id: feedRepeater
        model: FeedListFilterModel {
            doubleParentId: 0
            storage: folders ? localstorage : null
            Component.onCompleted: if (folders && startPage) { load() }
        }
    }
}

