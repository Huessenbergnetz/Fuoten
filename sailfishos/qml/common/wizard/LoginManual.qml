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
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
import harbour.fuoten.api 1.0

Dialog {
    id: loginManualDialog

    canAccept: username.length > 0 && password.length > 0 && host.length > 0
    acceptDestination: Qt.resolvedUrl("LoginManualCheck.qml")

    onAccepted: {
        config.username = username.text
        config.password = password.text
        config.host = host.text
        config.serverPort = parseInt(port.text, 10)
        config.installPath = installPath.text
        config.useSSL = usessl.checked
        config.ignoreSSLErrors = ignoresslerrors.checked
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            serverStatus.execute()
        }
    }

    GetServerStatus {
        id: serverStatus
    }

    SilicaFlickable {
        id: loginManualFlick
        anchors.fill: parent
        contentHeight: loginManualGrid.height + loginManualHeader.height

        VerticalScrollDecorator { flickable: loginManualFlick; page: loginManualDialog }

        PullDownMenu {
            flickable: loginManualFlick
            visible: serverStatus.setupPossible === GetServerStatus.LoginFlowV2
            MenuItem {
                //: pull down menu item in the setup wizard, login flow means to login to your Nextcloud by authenticating the app via an external browser window
                //% "Login flow"
                text: qsTrId("fuoten-loginflow-use-flow")
                onClicked: pageStack.replace(Qt.resolvedUrl("LoginFlowCheck.qml"))
            }
        }

        DialogHeader {
            id: loginManualHeader;
            //: dialog header title in the login flow setup wizard
            //% "Manual setup"
            title: qsTrId("fuoten-loginflow-manual-title");
            dialog: loginManualDialog;
            flickable: loginManualFlick
            defaultAcceptText: qsTrId("fuoten-loginflow-next")
        }

        GridLayout {
            id: loginManualGrid
            anchors { left: parent.left; right: parent.right; top: loginManualHeader.bottom; topMargin: account.contentHeight }

            columnSpacing: 0
            rowSpacing: Theme.paddingSmall

            columns: Screen.sizeCategory < Screen.Large ? (loginManualDialog.isLandscape ? 2 : 1) : (loginManualDialog.isLandscape ? 4 : 2)

            Item {
                Layout.columnSpan: loginManualGrid.columns
                Layout.fillWidth: true
                Layout.preferredHeight: infoText.height

                Text {
                    id: infoText
                    anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                    wrapMode: Text.WordWrap
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeExtraSmall
                    //% "Set up your user account by entering your user name, your password and the host address of the server your ownCloud/Nextcloud is running on. If your ownCloud/Nextcloud is installed in a subdirectory on your server, you have to enter the installation path as well. If your server uses an other port than the default port for HTTP(S), enter it in server port. If you use the standard ports or if you do not know what this is all about, keep the 0 at the server port."
                    text: qsTrId("id-account-setup-info")
                    textFormat: Text.PlainText
                }
            }

            TextField {
                id: username
                Layout.fillWidth: true
                //: Label and placeholder for a text input
                //% "User name"
                label: qsTrId("id-user-name"); placeholderText: label
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: password.focus = true
                text: config.username
            }

            PasswordField {
                id: password
                Layout.fillWidth: true
                showEchoModeToggle: true
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: host.focus = true
                text: config.password
            }

            TextField {
                id: host
                Layout.fillWidth: true
                //: Label for a text input
                //% "Host"
                label: qsTrId("id-host")
                //: Placeholder for a text input
                //% "Host: e.g. cloud.example.com"
                placeholderText: qsTrId("id-host-placeholder")
                inputMethodHints: Qt.ImhUrlCharactersOnly
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: installPath.focus = true
                text: config.host
            }

            TextField {
                id: installPath
                Layout.fillWidth: true
                //: Label and placeholder for a text input
                //% "Installation path"
                label: qsTrId("id-server-path"); placeholderText: label
                inputMethodHints: Qt.ImhUrlCharactersOnly
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: port.focus = true
                text: config.installPath
            }

            TextField {
                id: port
                Layout.fillWidth: true
                //: Label and placeholder for a text input
                //% "Server port"
                label: qsTrId("id-server-port"); placeholderText: label
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 0; top: 65536 }
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                EnterKey.onClicked: port.focus = false
                text: config.serverPort
            }

            Item {
                Layout.fillWidth: true
                Layout.columnSpan: Screen.sizeCategory < Screen.Large ? 1 : (loginManualDialog.isLandscape ? 3 : 1)
                height: 1
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: usessl.implicitHeight
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                TextSwitch {
                    id: usessl
                    //: Label for a switch
                    //% "Use HTTPS (SSL/TLS) connection"
                    text: qsTrId("id-use-ssl-text")
                    //: Description for a switch
                    //% "Because the News App requires to send your username and password with every request, you should keep this enabled to use an encrypted connection, when your server supports or even requires encryption."
                    description: qsTrId("id-use-ssl-desc")
                    checked: config.useSSL
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: ignoresslerrors.implicitHeight
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                TextSwitch {
                    id: ignoresslerrors
                    //: Label for a switch
                    //% "Ignore SSL errors"
                    text: qsTrId("id-ignore-ssl-errs-text")
                    //: Description for a switch
                    //% "Only ignore SSL errors when you really know what you are doing. Ignoring SSL errors is a big security risk."
                    description: qsTrId("id-ignore-ssl-errs-desc")
                    enabled: usessl.checked
                    checked: config.ignoreSSLErrors
                }
            }
        }
    }
}

