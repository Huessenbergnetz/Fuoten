/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/qml/phone/cover/CoverPage.qml
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
        visible: !synchronizer.inOperation && !cc.article

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
        visible: !synchronizer.inOperation && !cc.article
        wrapMode: Text.WordWrap
        lineHeight: 0.7
    }

    Label {
        id: updateRunning
        color: Theme.highlightColor
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
        visible: !synchronizer.inOperation && cc.article

        Label {
            width: parent.width
            truncationMode: TruncationMode.Fade
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.secondaryColor
            maximumLineCount: 1
            text: cc.article ? cc.article.feedTitle : ""
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
            text: cc.article ? cc.article.title : ""
        }

        Label {
            width: parent.width
            truncationMode: TruncationMode.Fade
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.secondaryColor
            maximumLineCount: 1
            text: cc.article ? (cc.article.humanPubDate + " | " + cc.article.humanPubTime) : ""
        }
    }

    CoverActionList {
        enabled: !synchronizer.inOperation

        CoverAction {
            iconSource: "image://theme/icon-cover-sync"
            onTriggered: synchronizer.sync()
        }
    }
}


