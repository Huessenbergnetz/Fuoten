/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0

Dialog {
    id: loginFlowSetup

    backNavigation: false
    canAccept: serverAddress.length > 3
    acceptDestination: Qt.resolvedUrl("LoginFlowCheck.qml")

    onAccepted: {
        var _url = usessl.checked ? "https://" : "http://"
        var _address = serverAddress.text
        _address.replace(/https?:\/\//i, "")
        _url += _address
        config.setServerUrl(_url)
        config.ignoreSSLErrors = ignoresslerrors.checked
    }

    SilicaFlickable {
        id: loginFlowSetupFlick
        anchors.fill: parent
        contentHeight: loginFlowSetupHeader.height + loginFlowSetupGrid.height

        VerticalScrollDecorator { flickable: loginFlowSetupFlick; page: loginFlowSetup }

        DialogHeader {
            id: loginFlowSetupHeader
            //: dialog header title in the login flow setup wizard
            //% "Connect your Nextcloud"
            title: qsTrId("fuoten-loginflow-setup-title")
            dialog: loginFlowSetup
            flickable: loginFlowSetupFlick
            //: dialog header in the login flow to go to the next step
            //% "Next"
            defaultAcceptText: qsTrId("fuoten-loginflow-next")
        }

        GridLayout {
            id: loginFlowSetupGrid
            anchors { left: parent.left; right: parent.right; top: loginFlowSetupHeader.bottom }

            columnSpacing: 0
            rowSpacing: Theme.paddingSmall

            columns: Screen.sizeCategory < Screen.Large ? (loginFlowSetup.isLandscape ? 2 : 1) : (loginFlowSetup.isLandscape ? 4 : 2)

            Item {
                Layout.columnSpan: loginFlowSetupGrid.columns
                Layout.fillWidth: true
                Layout.preferredHeight: infoText.height

                Text {
                    id: infoText
                    anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                    wrapMode: Text.WordWrap
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeExtraSmall
                    //% "Please enter the address (domain and path) of your Nextcloud. It is the same address where you can access your Nextcloud with a web browser."
                    text: qsTrId("fuoten-loginflow-setup-info")
                    textFormat: Text.PlainText
                }
            }

            TextField {
                id: serverAddress
                Layout.columnSpan: loginFlowSetupGrid.columns
                Layout.fillWidth: true
                label: usessl.checked
                       //: Label for a text input, %1 will be replaced by http://… or https://…
                       //% "Server address %1"
                       ? qsTrId("fuoten-loginflow-server-address").arg("https://…")
                       : qsTrId("fuoten-loginflow-server-address").arg("http://…")
                placeholderText: "cloud.example.com"
                inputMethodHints: Qt.ImhUrlCharactersOnly
                EnterKey.enabled: text.length > 3
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: loginFlowSetup.accept()
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: usessl.implicitHeight
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                TextSwitch {
                    id: usessl
                    //: Label for a switch
                    //% "Use HTTPS (SSL/TLS) connection"
                    text: qsTrId("fuoten-loginflow-usessl")
                    //: Description for a switch
                    //% "Your Nextcloud should only be accessible via an encrypted connection (HTTPS) for security reasons. If you still want to establish an unencrypted connection, you can deactivate the connection encryption here."
                    description: qsTrId("fuoten-loginflow-usessl-desc")
                    checked: true
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
                    checked: false
                }
            }
        }
    }
}
