/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0

Page {
    id: tabletMainPage

    SilicaListView {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTrId("id-about")
            }

            MenuItem {
                text: qsTrId("id-settings")
                onClicked: pageStack.push(Qt.resolvedUrl("../../common/pages/Settings.qml"))
            }
            MenuItem {
                text: qsTrId("fuoten-synchronize")
                onClicked: synchronizer.sync()
            }
        }

        header: PageHeader {
            title: "Fuoten"
            page: tabletMainPage
        }
    }
}
