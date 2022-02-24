/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
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
