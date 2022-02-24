/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0

Page {
    id: wipeManagerPage

    PageHeader {
        id: wipeManagerPageHeader
        //: page header for the remote wipe
        //% "Remote wipe"
        title: qsTrId("fuoten-remote-wipe-title")
    }

    BusyIndicator {
        id: wipeManagerBusy
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: remotewipe.inOperation
        visible: running
    }

    Label {
        id: wipeManagerBusyLabel
        anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin; top: wipeManagerBusy.bottom; topMargin: Theme.paddingSmall }
        textFormat: Text.PlainText
        visible: wipeManagerBusy.visible
        horizontalAlignment: Text.AlignHCenter
        truncationMode: TruncationMode.Fade
        maximumLineCount: 1
        //% "Executing remote wipe request"
        text: qsTrId("fuoten-remote-wipe-executing")
    }


    Item {
        id: remoteWipeDone

        visible: deletingAccountPage.deleteSuccess

        anchors.centerIn: parent

        width: parent.width
        height: remoteWipeDoneTitle.height + Theme.paddingMedium + remoteWipeDoneMessage.height

        Label {
            id: remoteWipeDoneTitle
            anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin; top: parent.top }
            horizontalAlignment: Text.AlignHCenter
            textFormat: Text.PlainText
            truncationMode: TruncationMode.Fade
            maximumLineCount: 1
            //% "Remote wipe done"
            text: qsTrId("fuoten-remote-wipe-done-title")
        }

        Text {
            id: remoteWipeDoneMessage
            anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin; top: remoteWipeDoneTitle.bottom; topMargin: Theme.paddingMedium }
            font.pixelSize: Theme.fontSizeSmall
            color: Theme.primaryColor
            horizontalAlignment: Text.AlignHCenter
            textFormat: Text.PlainText
            wrapMode: Text.Wrap
            //% "The requested remote wipe has been done. Your local account and content data of Fuoten have been deleted."
            text: qsTrId("fuoten-remote-wipe-done-message")
        }
    }
}
