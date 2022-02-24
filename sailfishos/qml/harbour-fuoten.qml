/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0

ApplicationWindow
{
    id: fuoten
    initialPage: config.isFirstStart()
                 ? Qt.resolvedUrl("common/wizard/Welcome.qml")
                 : Screen.sizeCategory >= Screen.Large
                   ? Qt.resolvedUrl("tablet/pages/MainPage.qml")
                   : Qt.resolvedUrl("phone/pages/MainPage.qml")
    cover: Screen.sizeCategory >= Screen.Large
           ? Qt.resolvedUrl("tablet/cover/CoverPage.qml")
           : Qt.resolvedUrl("phone/cover/CoverPage.qml")
    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All

    Connections {
        target: pageStack
        onCurrentPageChanged: {
            if (!synchronizer.inOperation) {
                config.checkUpdate();
            }
        }
    }

    Connections {
        target: _fuotenDbusProxy
        onDbusActivate: {
            if (Qt.application.state !== Qt.ApplicationActive) {
                fuoten.activate()
            }
        }
    }

    Connections {
        target: wipemanager
        onWipeStarted: pageStack.replaceAbove(null, Qt.resolvedUrl("common/pages/WipeManager.qml"))
    }
}


