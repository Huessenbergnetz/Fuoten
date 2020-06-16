/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
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
}


