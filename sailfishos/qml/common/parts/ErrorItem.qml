/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
import harbour.fuoten 1.0

RowLayout {

    property bool highlighted: false
    property FuotenError error: null

    property real iconSize: Theme.iconSizeMedium
    property alias fontSize: errorText.font.pixelSize

    spacing: Theme.paddingMedium

    visible: error

    Image {
        source: "image://theme/icon-l-attention?" + (highlighted ? Theme.highlightColor : Theme.primaryColor)
        Layout.preferredWidth: iconSize
        Layout.preferredHeight: iconSize
        fillMode: Image.PreserveAspectFit
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
    }

    Text {
        id: errorText
        text: error ? error.text : ""
        color: highlighted ? Theme.highlightColor : Theme.primaryColor
        font.pixelSize: Theme.fontSizeSmall
        Layout.fillWidth: true
        wrapMode: Text.WordWrap
        maximumLineCount: 3
        elide: Text.ElideRight
    }
}

