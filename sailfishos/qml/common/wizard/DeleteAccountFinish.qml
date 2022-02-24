/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.fuoten 1.0
import harbour.fuoten.api 1.0

Page {
    id: deletingAccountPage

    backNavigation: false

    property bool deleteLocal: false
    property bool deleteRemote: false
    property bool deleteSuccess: false

    DeleteAppPassword {
        id: deleteAppPwd
        onSucceeded: {
            if (deleteLocal) {
                //% "Deleting local storage"
                deletingAccountBusyLabel.text = qsTrId("fuoten-deleting-account-busy-local")
                localstorage.clearStorage()
            } else {
                config.deleteAccount()
            }
        }
        onFailed: {
            if (deleteLocal) {
                deletingAccountBusyLabel.text = qsTrId("fuoten-deleting-account-busy-local")
                localstorage.clearStorage()
            } else {
                config.deleteAccount()
            }
        }
    }

    Connections {
        target: localstorage
        onStorageCleared: config.deleteAccount()
    }

    Connections {
        target: config
        onAccountDeleted: {
            deletingAccountBusy.running = false
            deletingAccountPage.deleteSuccess = true
        }
    }

//    onStatusChanged: console.log("DELETE LOCAL: " + deleteLocal + ", DELETE REMOTE: " + deleteRemote)
    onStatusChanged: {
        if (status === PageStatus.Active) {
            if (deleteRemote) {
                //% "Deleting application password"
                deletingAccountBusyLabel.text = qsTrId("fuoten-deleting-account-busy-remote")
                deleteAppPwd.execute()
            } else if (deleteLocal) {
                deletingAccountBusyLabel.text = qsTrId("fuoten-deleting-account-busy-local")
                localstorage.clearStorage()
            } else {
                config.deleteAccount()
            }
        }
    }

    PageHeader {
        id: deletingAccountPageHeader
        //: Page header when deleting account data
        //% "Deleting account"
        title: qsTrId("fuoten-deleting-account-title")
    }

    BusyIndicator {
        id: deletingAccountBusy
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: true
        visible: running
    }

    Label {
        id: deletingAccountBusyLabel
        anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin; top: deletingAccountBusy.bottom; topMargin: Theme.paddingSmall }
        textFormat: Text.PlainText
        visible: deletingAccountBusy.visible
        horizontalAlignment: Text.AlignHCenter
        truncationMode: TruncationMode.Fade
        maximumLineCount: 1
    }


    Item {
        id: deletingAccountSuccess

        visible: deletingAccountPage.deleteSuccess

        anchors.centerIn: parent

        width: parent.width
        height: deletingAccountSuccessTitle.height + Theme.paddingMedium + deletingAccountSuccessMessage.height

        Label {
            id: deletingAccountSuccessTitle
            anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin; top: parent.top }
            horizontalAlignment: Text.AlignHCenter
            textFormat: Text.PlainText
            truncationMode: TruncationMode.Fade
            maximumLineCount: 1
            //% "Account deleted"
            text: qsTrId("fuoten-deleting-account-success-title")
        }

        Text {
            id: deletingAccountSuccessMessage
            anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin; top: deletingAccountSuccessTitle.bottom; topMargin: Theme.paddingMedium }
            font.pixelSize: Theme.fontSizeSmall
            color: Theme.primaryColor
            horizontalAlignment: Text.AlignHCenter
            textFormat: Text.PlainText
            wrapMode: Text.Wrap
            //% "Your account was successfully deleted, please close the application. If you want to create a new account, simply restart the application."
            text: qsTrId("fuoten-deleting-account-success-message")
        }
    }
}
