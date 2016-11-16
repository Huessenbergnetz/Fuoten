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
