/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import "../../common/parts"

Page {
    id: feedListPage

    property alias folder: flv.folder

    FeedsListView {
        id: flv
        page: feedListPage
        anchors {
            fill: parent
            bottomMargin: syncPanel.height
        }
    }

    SyncPanel {
        id: syncPanel
    }
}


