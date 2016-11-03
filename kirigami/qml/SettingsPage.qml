/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/src/main.cpp
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

import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import org.kde.kirigami 1.0 as Kirigami
import buschtrommel.fuoten 1.0

Kirigami.ScrollablePage {

    objectName: "settingsPage"

    //% "Settings"
    title: qsTrId("id-settings")

    GridLayout {
        id: settingsGrid
        columns: 2

        columnSpacing: Kirigami.Units.largeSpacing
        rowSpacing: Kirigami.Units.largeSpacing

        AccountValidator {
            id: accValidator
            configuration: config
        }

        Kirigami.Heading {
            //% "User account"
            text: qsTrId("id-user-account")
            Layout.columnSpan: settingsGrid.columns
            level: 2
        }

        LabeledTextField {
            id: username
            //% "User name"
            label: qsTrId("id-user-name"); placeholderText: label
            text: config.username
        }

        LabeledTextField {
            id: password
            //% "Password"
            label: qsTrId("id-password"); placeholderText: label
            echoMode: TextInput.Password
            text: config.password
        }

        LabeledTextField {
            id: host
            //% "Host"
            label: qsTrId("id-host")
            //% "Host: e.g. cloud.example.com"
            placeholderText: qsTrId("id-host-placeholder")
            inputMethodHints: Qt.ImhUrlCharactersOnly
            text: config.host
        }

        LabeledTextField {
            id: installPath
            //% "Installation path"
            label: qsTrId("id-server-path"); placeholderText: label
            inputMethodHints: Qt.ImhUrlCharactersOnly
            text: config.installPath
        }

        LabeledTextField {
            id: port
            //% "Server port"
            label: qsTrId("id-server-port"); placeholderText: label
            inputMethodHints: Qt.ImhDigitsOnly
            validator: IntValidator { bottom: 0; top: 65536 }
            text: config.serverPort
        }

        Item {
            Layout.fillWidth: true
            height: 1
        }

        TextSwitch {
            id: usessl
            Layout.fillWidth: true
            //% "Use HTTPS (SSL/TLS) connection"
            label: qsTrId("id-use-ssl-text")
            //% "Because the News App requires to send your username and password with every request, you should keep this enabled to use an encrypted connection, when your server supports or even requires encryption."
            description: qsTrId("id-use-ssl-desc")
            checked: config.useSSL
        }

        TextSwitch {
            id: ignoresslerrors
            Layout.fillWidth: true
            //% "Ignore SSL errors"
            label: qsTrId("id-ignore-ssl-errs-text")
            //% "Only ignore SSL errors when you really know what you are doing. Ignoring SSL errors is a big security risk."
            description: qsTrId("id-ignore-ssl-errs-desc")
            enabled: usessl.checked
            checked: config.ignoreSSLErrors
        }

        Item {
            Layout.columnSpan: settingsGrid.columns
            Layout.fillWidth: true

            Button {
                text: "Check"
                onClicked: {
                    config.username = username.text
                    config.password = password.text
                    config.host = host.text
                    config.installPath = installPath.text
                    config.serverPort = port.text.length > 0 ? parseInt(port.text) : 0
                    config.useSSL = usessl.checked
                    config.ignoreSSLErrors = ignoresslerrors.checked
                    accValidator.start()
                }
            }
        }
    }
}
