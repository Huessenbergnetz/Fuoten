/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
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


