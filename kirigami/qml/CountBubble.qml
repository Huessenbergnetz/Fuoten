/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * kirigami/qml/CountBubble.qml
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
import org.kde.kirigami 1.0 as Kirigami

Item {
    id: countBubble

    property int value: 0
    property alias color: count.color

    property real fontSize: Kirigami.Theme.defaultFont.pointSize

    function getBubbleWidth() {
        if (countBubble.value < 10)
            return fontSize*0.625*(2+0.5);
        else if (countBubble.value < 100)
            return fontSize*0.625*(2+0.5);
        else if (countBubble.value < 1000)
            return fontSize*0.625*(3+0.5);
        else
            return fontSize*0.625*(4+0.5);
    }

//    width: countBubble.getBubbleWidth()
    width: count.contentWidth + (fontSize * 1.5)
    height: fontSize*2.5

//    onValueChanged: width = getBubbleWidth()

    Rectangle {
        id: backgroundRect
        width: parent.width
        height: parent.height
        color: Kirigami.Theme.highlightColor
//        border.color: Theme.primaryColor
//        border.width: 0
        radius: 10
//        opacity: 0.3
    }

    Kirigami.Label {
        id: count
        anchors.centerIn: backgroundRect
        text: countBubble.value
        font.pointSize: countBubble.fontSize
        color: "white"
    }
}
