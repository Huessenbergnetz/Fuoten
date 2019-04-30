/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2019 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * kirigami/qml/TextSwitch.qml
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

MouseArea {
    id: textSwitchRoot

    property alias checked: switchItem.checked
    property alias label: switchLabel.text
    property alias description: descText.text

    implicitHeight: textSwitchRow.height

    onClicked: switchItem.checked = !switchItem.checked


    cursorShape: Qt.PointingHandCursor

    RowLayout {
        id: textSwitchRow
        width: parent.width
        spacing: Kirigami.Units.smallSpacing

        Switch {
            id: switchItem
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.preferredWidth: implicitWidth
            enabled: textSwitchRoot.enabled
        }

        ColumnLayout {
            Kirigami.Label {
                id: switchLabel
                enabled: switchItem.checked && textSwitchRoot.enabled
                Layout.fillWidth: true
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.1
            }

            Kirigami.Label {
                id: descText
                Layout.fillWidth: true
                color: switchItem.checked && textSwitchRoot.enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 0.9
            }
        }
    }

}
