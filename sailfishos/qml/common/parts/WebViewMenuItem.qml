/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0

BackgroundItem {
    id: webViewMenuItem
    width: parent ? parent.width : Screen.width
    height: Theme.itemSizeSmall

    property alias icon: menuPanelItemIcon.source
    property alias text: menuPanelItemText.text

    Icon {
        id: menuPanelItemIcon
        anchors {
            left: parent.left
            leftMargin: Theme.horizontalPageMargin
            verticalCenter: parent.verticalCenter
        }
        width: Theme.iconSizeMedium; height: Theme.iconSizeMedium
        highlighted: webViewMenuItem.highligted
        opacity: webViewMenuItem.enabled ? 1.0 : Theme.opacityLow
    }

    Label {
        id: menuPanelItemText
        anchors {
            left: menuPanelItemIcon.right
            leftMargin: Theme.paddingMedium
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
            verticalCenter: parent.verticalCenter
        }
        highlighted: webViewMenuItem.highlighted
        font.pixelSize: Theme.fontSizeSmall
        truncationMode: TruncationMode.Fade
        opacity: webViewMenuItem.enabled ? 1.0 : Theme.opacityLow
    }
}
