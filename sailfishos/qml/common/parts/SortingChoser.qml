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
import harbour.fuoten 1.0

ComboBox {
    id: sortingChoser
    property int type: Fuoten.Folder
    property bool mainView: false
    //% "Sort by"
    label: qsTrId("fuoten-sort-by-label")
    //% "Choose the role you want to sort the content by."
    description: qsTrId("fuoten-sort-by-desc")
    menu: ContextMenu {
        MenuItem {
            //% "Database ID"
            text: qsTrId("fuoten-sort-database-id")
            readonly property int value: Fuoten.ID
        }
        MenuItem {
            //% "Title"
            text: qsTrId("fuoten-sort-title")
            readonly property int value: Fuoten.Name
        }
        MenuItem {
            //% "Time"
            text: qsTrId("fuoten-sort-time")
            readonly property int value: Fuoten.Time
            visible: sortingChoser.type === Fuoten.Feed && !mainView
        }
        MenuItem {
            //% "Unread count"
            text: qsTrId("fuoten-sort-unread-count")
            readonly property int value: Fuoten.UnreadCount
            visible: sortingChoser.type === Fuoten.Folder || mainView
        }
        MenuItem {
            //% "Feed count"
            text: qsTrId("fuoten-sort-feed-count")
            readonly property int value: Fuoten.FeedCount
            visible: sortingChoser.type === Fuoten.Folder
        }
    }
}

