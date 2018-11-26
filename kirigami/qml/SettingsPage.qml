/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2018 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * kirigami/qml/SettingsPage.qml
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
            text: qsTrId("id-user-account")
            Layout.columnSpan: settingsGrid.columns
            level: 2
        }

        LabeledTextField {
            id: username
            label: qsTrId("id-user-name"); placeholderText: label
            text: config.username
        }

        LabeledTextField {
            id: password
            label: qsTrId("id-password"); placeholderText: label
            echoMode: TextInput.Password
            text: config.password
        }

        LabeledTextField {
            id: host
            label: qsTrId("id-host")
            placeholderText: qsTrId("id-host-placeholder")
            inputMethodHints: Qt.ImhUrlCharactersOnly
            text: config.host
        }

        LabeledTextField {
            id: installPath
            label: qsTrId("id-server-path"); placeholderText: label
            inputMethodHints: Qt.ImhUrlCharactersOnly
            text: config.installPath
        }

        LabeledTextField {
            id: port
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
            label: qsTrId("id-use-ssl-text")
            description: qsTrId("id-use-ssl-desc")
            checked: config.useSSL
        }

        TextSwitch {
            id: ignoresslerrors
            Layout.fillWidth: true
            label: qsTrId("id-ignore-ssl-errs-text")
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
