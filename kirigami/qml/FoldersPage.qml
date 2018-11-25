/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/src/main.cpp
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

import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import org.kde.kirigami 1.0 as Kirigami
import buschtrommel.fuoten 1.0
import buschtrommel.fuoten.models 1.0

Kirigami.ScrollablePage {

    //% "Folders"
    title: qsTrId("id-folders")

    objectName: "foldersPage"

    ListView {
        model: FolderListFilterModel {
            storage: localstorage
            Component.onCompleted: load()
        }

        delegate: Kirigami.SwipeListItem {

            onClicked: fuotenWindow.pageStack.push(Qt.resolvedUrl("FeedsPage.qml"), {folder: model.display})

            RowLayout {
                anchors { left: parent.left; leftMargin: Kirigami.Units.largeSpacing; right: parent.right; rightMargin: Kirigami.Units.largeSpacing * 1.5 }

                ColumnLayout {
                    Layout.fillWidth: true

                    Kirigami.Heading {
                        Layout.fillWidth: true
                        level: 2
                        text: model.display.name
                    }

                    Kirigami.Label {
                        Layout.fillWidth: true
                        //% "%n feed(s)"
                        text: qsTrId("fuoten-feeds-count", model.display.feedCount)
                    }
                }

                CountBubble {
                    value: model.display.unreadCount
                }
            }

            actions: [
                Kirigami.Action {
                    iconName: "edit-delete"
                    tooltip: qsTrId("fuoten-delete-folder")
                }

            ]
        }
    }
}
