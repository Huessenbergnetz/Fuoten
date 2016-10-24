/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
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

Item {
    id: countBubble

    property int value: 0
    property alias color: count.color

    property int fontSize: Screen.sizeCategory >= Screen.Large
                          ? Theme.fontSizeSmall
                          : Theme.fontSizeExtraSmall

    function getBubbleWidth() {
        if (countBubble.value < 10)
            return fontSize*0.625*(1+0.5);
        else if (countBubble.value < 100)
            return fontSize*0.625*(2+0.5);
        else if (countBubble.value < 1000)
            return fontSize*0.625*(3+0.5);
        else
            return fontSize*0.625*(4+0.5);
    }

    onValueChanged: {
        width = getBubbleWidth()
    }

    width: countBubble.getBubbleWidth()
    height: fontSize*1.2

    Rectangle {
        id: backgroundRect
        width: parent.width
        height: parent.height
        color: Theme.secondaryColor
        border.color: Theme.primaryColor
        border.width: 1
        radius: 10
        opacity: 0.3
    }

    Text {
        id: count
        anchors.centerIn: backgroundRect
        text: countBubble.value
        font.pixelSize: fontSize
        opacity: 1
        color: Theme.primaryColor
    }
}

