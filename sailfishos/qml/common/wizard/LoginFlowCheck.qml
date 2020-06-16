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
import harbour.fuoten.api 1.0
import "../parts"

Dialog {
    id: loginFlowCheck

    canAccept: serverStatus.setupPossible > GetServerStatus.NotPossible
    acceptDestination: serverStatus.setupPossible === GetServerStatus.LoginFlowV2
                       ? Qt.resolvedUrl("LoginFlowPolling.qml")
                       : serverStatus.setupPossible !== GetServerStatus.NotPossible
                         ? Qt.resolvedUrl("LoginManual.qml")
                         : undefined
    property bool checkPerformed: false

    onStatusChanged: {
        if (status === PageStatus.Active) {
            serverStatus.execute()
        }
    }

    GetServerStatus {
        id: serverStatus
    }

    SilicaFlickable {
        id: loginFlowCheckFlick
        anchors.fill: parent
        contentHeight: loginFlowCheckHeader.height + loginFlowCheckCol.height

        VerticalScrollDecorator { flickable: loginFlowCheckFlick; page: loginFlowCheck }

        PullDownMenu {
            flickable: loginFlowCheckFlick
            MenuItem {
                //: pull down menu item in the setup wizard
                //% "Manual login"
                text: qsTrId("fuoten-loginflow-enter-manual")
                enabled: !serverStatus.inOperation
                visible: serverStatus.setupPossible !== GetServerStatus.Manual
                onClicked: {
                    loginFlowCheck.acceptDestination = Qt.resolvedUrl("LoginManual.qml")
                    loginFlowCheck.canAccept = true
                    loginFlowCheck.accept()
                }
            }
            MenuItem {
                //: pull down menu item in the setup wizard
                //% "Check again"
                text: qsTrId("fuoten-loginflow-check-again")
                enabled: !serverStatus.inOperation
                onClicked: serverStatus.execute()
            }
        }

        DialogHeader {
            id: loginFlowCheckHeader
            //: dialog header title in the setup wizard
            //% "Check server"
            title: qsTrId("fuoten-loginflow-check-title")
            dialog: loginFlowCheck
            flickable: loginFlowCheckFlick
            defaultAcceptText: qsTrId("fuoten-loginflow-next")
            //: dialog header in the login flow to go to the previous step
            //% "Back"
            defaultCancelText: qsTrId("fuoten-loginflow-back")
        }

        Column {
            id: loginFlowCheckCol
            anchors { left: parent.left; right: parent.right; top: loginFlowCheckHeader.bottom }

            BusyIndicator {
                id: loginFlowCheckBusy
                anchors.horizontalCenter: parent.horizontalCenter
                size: BusyIndicatorSize.Large
                running: visible
                visible: serverStatus.inOperation
            }

            DetailItem {
                //: label for a detail item showing the server name, should not be too long
                //% "Server"
                label: qsTrId("fuoten-loginflow-server")
                value: serverStatus.productname
                visible: !serverStatus.inOperation && !serverStatus.error && value.length
            }

            DetailItem {
                //: Label for a detail item showing the nextcloud version, should not be too long
                //% "Version"
                label: qsTrId("fuoten-loginflow-version")
                value: serverStatus.versionString
                visible: !serverStatus.inOperation && !serverStatus.error && value.length
            }

            DetailItem {
                //: label for a detail item showing if the nextcloud is fully installed, value will be either yes or no
                //% "Installed"
                label: qsTrId("fuoten-loginflow-installed")
                value: serverStatus.isInstalled
                       //% "yes"
                       ? qsTrId("fuoten-yes")
                       //% "no"
                       : qsTrId("fuoten-no")
                visible: !serverStatus.inOperation && !serverStatus.error && serverStatus.versionString.length
            }

            DetailItem {
                //: label for a detail item showing if the nexctloud is in maintenance mode, value will be eiter yes or no
                //% "Maintenance mode"
                label: qsTrId("fuoten-loginflow-inmaintenance")
                value: serverStatus.isInMaintenance
                       ? qsTrId("fuoten-yes")
                       : qsTrId("fuoten-no")
                visible: !serverStatus.inOperation && !serverStatus.error && serverStatus.versionString.length
            }

            Text {
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
                //% "Your version of Nextcloud is not yet able to authorize an application through an external browser window. In the next step you will have the option to enter a manually created application password to access your Nextcloud. To create such a password, please log in to your Nextcloud via the web interface and create a new application password in the “Security” section of the settings."
                text: qsTrId("fuoten-loginflow-check-manual-setup-info")
                visible: !serverStatus.inOperation && !serverStatus.error && serverStatus.setupPossible > GetServerStatus.NotPossible && serverStatus.setupPossible < GetServerStatus.LoginFlowV2
                textFormat: Text.PlainText
            }

            Text {
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
                //: %1 will be replaced by the name of the application and the device it runs on, e.g.: "Fuoten (Xperia X)"
                //% "The next step is to open an external browser window with the login page of your Nextcloud. By logging in you authorize this application to use your account there. You can revoke this authorization at any time in your Nextcloud settings under “Security”. The name of the application to be authorized is <b>%1</b>."
                text: qsTrId("fuoten-loginflow-check-v2-setup-info").arg(config.getLoginFlowUserAgent())
                visible: !serverStatus.inOperation && !serverStatus.error && serverStatus.setupPossible === GetServerStatus.LoginFlowV2
                textFormat: Text.StyledText
            }

            ErrorItem {
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                error: serverStatus.error
                visible: serverStatus.error && !serverStatus.inOperation
            }
        }
    }
}
