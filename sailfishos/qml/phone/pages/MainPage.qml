/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/qml/phone/pages/MainPage.qml
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
import harbour.fuoten 1.0
import "../../common/parts"

Page {
    id: phoneMainPage

    property bool contextAttached: false

    function loadMainView() {
        if (contextAttached) {
            pageStack.popAttached(phoneMainPage, PageStackAction.Immediate);
        }

        if (config.mainViewType === Fuoten.Folder) {
            mainPageLoader.setSource(Qt.resolvedUrl("FolderListView.qml"), {page: phoneMainPage, startPage: true})
        } else {
            mainPageLoader.setSource(Qt.resolvedUrl("FeedsListView.qml"), {page: phoneMainPage, startPage: true})
        }

        contextAttached = true
    }

    Component.onCompleted: loadMainView()

    Connections {
        target: config
        onMainViewTypeChanged: loadMainView()
    }

    Loader {
        id: mainPageLoader
        anchors { fill: parent; bottomMargin: syncPanel.height }
    }

    SyncPanel {
        id: syncPanel
    }
}


