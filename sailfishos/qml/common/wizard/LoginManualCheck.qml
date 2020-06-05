/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2019 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/qml/common/wizard/LoginManualCheck.qml
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
import "../parts"

Dialog {
    id: loginManualCheck
    canAccept: config.isAccountValid && !account.inOperation && !account.error
    backNavigation: !config.isAccountValid
    acceptDestination: Screen.sizeCategory >= Screen.Large
                       ? Qt.resolvedUrl("../../tablet/pages/MainPage.qml")
                       : Qt.resolvedUrl("../../phone/pages/MainPage.qml")
    acceptDestinationAction: PageStackAction.Replace
    acceptDestinationReplaceTarget: null

    onAccepted: {
        config.setCurrentVersion()
        synchronizer.sync()
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            account.validate()
        }
    }

    SilicaFlickable {
        id: loginManualCheckFlick
        anchors.fill: parent
        contentHeight: loginManualCheckHeader.height + account.height

        VerticalScrollDecorator { flickable: loginManualCheckFlick; page: loginManualCheck }

        PullDownMenu {
            flickable: loginManualCheckFlick
            MenuItem {
                text: qsTrId("fuoten-loginflow-polling-retry")
            }
        }

        DialogHeader {
            id: loginManualCheckHeader
            dialog: loginManualCheck
            flickable: loginManualCheckFlick
            defaultAcceptText: qsTrId("fuoten-wizard-finish")
            title: account.inOperation
                   //: Dialog title in the login flow while checking manual account setup
                   //% "Checking account"
                   ? qsTrId("fuoten-loginflow-manual-check-account")
                   : account.error
                     //: Dialog title in the login flow if manual account setup failed
                     //% "Setup failed"
                     ? qsTrId("fuoten-loginflow-manual-check-failed")
                     : config.isAccountValid
                       //: Dialog title in the login flow if manual account setup succeeded
                       //% "Setup finished"
                       ? qsTrId("fuoten-loginflow-manual-check-success")
                       : qsTrId("fuoten-loginflow-manual-check-account")
        }

        AccountItem {
            id: account
            anchors { left: parent.left; right: parent.right; top: loginManualCheckHeader.bottom }
            enabled: false
        }
    }
}
