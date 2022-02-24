/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import "../../common/parts"

Page {
    id: articlesListPage

    property alias folder: alv.folder
    property alias feed: alv.feed
    property alias context: alv.contextType

    ArticlesListView {
        id: alv
        page: articlesListPage
    }

    SyncPanel {
        id: syncPanel
    }
}

