/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0

CoverBackground {
    id: cov

    onStatusChanged: {
        if (status === Cover.Active) {
            if (!synchronizer.inOperation) {
                config.checkUpdate();
            }
        }
    }

    Column {
        id: countCol
        anchors { top: parent.top; left: parent.left; right: parent.right; topMargin: Theme.paddingLarge; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
        visible: !synchronizer.inOperation && !covercon.article

        Text {
            id: count
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeHuge
            font.weight: Font.Light
            textFormat: Text.PlainText
            text: localstorage.totalUnread
        }

        Text {
            //% "Unread articles"
            text: qsTrId("fuoten-unread-articles")
            id: countText
            width: parent.width
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeSmall
            textFormat: Text.StyledText
            lineHeight: 0.7
            wrapMode: Text.WordWrap
        }
    }

    Label {
        id: lastUpdText
        color: Theme.highlightColor
        anchors { top: countCol.bottom; left: parent.left; right: parent.right; topMargin: Theme.paddingLarge * 1.5; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
        font.pixelSize: Theme.fontSizeLarge
        text: config.humanLastSync
        width: parent.width
        textFormat: Text.PlainText
        visible: !synchronizer.inOperation && !covercon.article
        wrapMode: Text.WordWrap
        lineHeight: 0.7
    }

    Label {
        id: updateRunning
        color: Theme.highlightColor
        //: Text on cover and in the menu indicating that synchronization is running
        //% "Synchronizing"
        text: qsTrId("fuoten-synchronizing")
        anchors { verticalCenter: parent.verticalCenter; left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
        width: parent.width
        visible: synchronizer.inOperation
        font.pixelSize: Theme.fontSizeLarge
        maximumLineCount: 1
        truncationMode: TruncationMode.Fade
        textFormat: Text.PlainText
        Timer {
            property int angle: 0

            running: cov.status === Cover.Active && synchronizer.inOperation
            interval: 50
            repeat: true

            onTriggered: {
                var a = angle;
                var opacity = 0.5 + 0.5 * Math.sin(angle * (Math.PI / 180.0));
                // Workaround for Silica's bug on opacity != 0 on cover page when label is truncated with fading
                parent.color = Theme.rgba(Theme.highlightColor, opacity)
                angle = (angle + 10) % 360;
            }
        }
    }

    Column {
        anchors { left: parent.left; right: parent.right; leftMargin: Theme.paddingMedium; rightMargin: Theme.paddingMedium; top: parent.top; topMargin: Theme.paddingSmall }
        visible: !synchronizer.inOperation && covercon.article

        Label {
            width: parent.width
            truncationMode: TruncationMode.Fade
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.secondaryColor
            maximumLineCount: 1
            text: covercon.article ? covercon.article.feedTitle : ""
        }

        Separator {
            width: parent.width
            color: Theme.secondaryColor
        }

        Item {
            width: parent.width
            height: Theme.paddingSmall
        }

        Label {
            width: parent.width
            color: Theme.primaryColor
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            elide: Text.ElideRight
            font.pixelSize: Theme.fontSizeSmall
            maximumLineCount: 6
            text: covercon.article ? covercon.article.title : ""
        }

        Label {
            width: parent.width
            truncationMode: TruncationMode.Fade
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.secondaryColor
            maximumLineCount: 1
            text: covercon.article ? (covercon.article.humanPubDate + " | " + covercon.article.humanPubTime) : ""
        }
    }

    CoverActionList {
        enabled: !synchronizer.inOperation && !covercon.article

        CoverAction {
            iconSource: "image://theme/icon-cover-sync"
            onTriggered: synchronizer.sync()
        }
    }

    CoverActionList {
        enabled: !synchronizer.inOperation && covercon.article

        CoverAction {
            iconSource: "image://theme/icon-s-clipboard"
            onTriggered: Clipboard.text = covercon.article.url.toString()
        }
    }
}


