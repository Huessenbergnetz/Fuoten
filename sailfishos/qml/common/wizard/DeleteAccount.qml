/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0

Dialog {
    id: deleteAccount

    acceptDestination: Qt.resolvedUrl("DeleteAccountFinish.qml")
    acceptDestinationAction: PageStackAction.Replace

    onAccepted: {
        acceptDestinationInstance.deleteLocal = deleteAccountLocal.checked
        acceptDestinationInstance.deleteRemote = deleteAccountRemote.checked
    }

    SilicaFlickable {
        id: deleteAccountFlick
        anchors.fill: parent
        contentHeight: deleteAccountHeader.height + deleteAccountCol.height

        VerticalScrollDecorator { flickable: deleteAccountFlick; page: deleteAccount }

        DialogHeader {
            id: deleteAccountHeader
            //: dialog header title when deleting the nextcloud account and pull down menu entry
            //% "Delete account"
            title: qsTrId("fuoten-deleteaccount-title")
            dialog: deleteAccount
            flickable: deleteAccountFlick
            //: dialog header accept text when deleting account
            //% "Delete"
            defaultAcceptText: qsTrId("fuoten-deleteaccount-delete")
        }

        Column {
            id: deleteAccountCol

            anchors { left: parent.left; right: parent.right; top: deleteAccountHeader.bottom }

            TextSwitch {
                id: deleteAccountLocal
                //: Label for a text switch when deleting account
                //% "Delete local data"
                text: qsTrId("fuoten-deleteaccount-local-text")
                //: Description for a text switch when deleting account
                //% "Also deletes all local data, including the database with all folders, feeds and articles."
                description: qsTrId("fuoten-deleteaccount-local-desc")
                checked: true
            }

            TextSwitch {
                id: deleteAccountRemote
                //: Label for a text switch when deleting account
                //% "Delete app password"
                text: qsTrId("fuoten-deleteaccount-remote-text")
                //: Description for a text switch when deleting account
                //% "Also deletes the application password on the server, if one has been set."
                description: qsTrId("fuoten-deleteaccount-remote-desc")
                checked: true
            }
        }
    }
}
