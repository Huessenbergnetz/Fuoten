/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
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
