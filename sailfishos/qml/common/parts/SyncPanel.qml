/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
import QtQuick 2.2
import Sailfish.Silica 1.0
import "../../common/parts"

Rectangle {
    id: syncPanel
    height: open ? Theme.itemSizeLarge : 0
    anchors { left: parent.left; right: parent.right; bottom: parent.bottom }

    readonly property bool open: synchronizer.inOperation || synchronizer.error

    Behavior on height {
        id: horizontalBehavior
        NumberAnimation {
            id: horizontalAnimation
            duration: 500; easing.type: Easing.OutQuad
        }
    }

    gradient: Gradient {
        GradientStop { position: 0.0; color: Theme.rgba(Theme.highlightBackgroundColor, 0.15) }
        GradientStop { position: 1.0; color: Theme.rgba(Theme.highlightBackgroundColor, 0.3) }
    }

    ProgressBar {
        anchors.centerIn: parent
        value: synchronizer.progress
        label: synchronizer.currentAction
        width: parent.width
        visible: !synchronizer.error && synchronizer.inOperation
    }

    BackgroundItem {
        id: eItemBg
        enabled: synchronizer.error
        anchors.fill: parent
        contentHeight: parent.height
        onClicked: synchronizer.clearError()
    }

    ErrorItem {
        id: eItem
        highlighted: eItemBg.highlighted
        anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
        error: synchronizer.error
    }

}
