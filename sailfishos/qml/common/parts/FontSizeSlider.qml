/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2018 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/qml/common/parts/FontSizeSlider.qml
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

import QtQuick 2.0

Slider {
    id: fontSizeSlider
    width: parent.width
    minimumValue: Theme.fontSizeTiny
    maximumValue: Theme.fontSizeHuge
    stepSize: 1
    onSliderValueChanged: {
        switch(sliderValue) {
        case Theme.fontSizeTiny:
            //: Name of a font size
            //% "Tiny"
            valueText = qsTrId("fuoten-font-size-tiny")
            break;
        case Theme.fontSizeExtraSmall:
            //: Name of a font size
            //% "Extra small"
            valueText = qsTrId("fuoten-font-size-extra-small")
            break;
        case Theme.fontSizeSmall:
            //: Name of a font size
            //% "Small"
            valueText = qsTrId("fuoten-font-size-small")
            break;
        case Theme.fontSizeMedium:
            //: Name of a font size
            //% "Medium"
            valueText = qsTrId("fuoten-font-size-medium")
            break;
        case Theme.fontSizeLarge:
            //: Name of a font size
            //% "Large"
            valueText = qsTrId("fuoten-font-size-large")
            break;
        case Theme.fontSizeExtraLarge:
            //: Name of a font size
            //% "Extra large"
            valueText = qsTrId("fuoten-font-size-extra-large")
            break;
        case Theme.fontSizeHuge:
            //: Name of a font size
            //% "Huge"
            valueText = qsTrId("fuoten-font-size-huge")
            break;
        default:
            valueText = sliderValue + "px"
            break;
        }
        fontSizeSliderTimer.restart()
    }

    signal changeTriggered

    Timer {
        id: fontSizeSliderTimer
        interval: 800
//        onTriggered: config.articleFontSize = Math.floor(fontSizeSlider.sliderValue)
        onTriggered: fontSizeSlider.changeTriggered()
    }
}
