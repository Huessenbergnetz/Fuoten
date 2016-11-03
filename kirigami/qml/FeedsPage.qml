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
import buschtrommel.fuoten.items 1.0

Kirigami.ScrollablePage {

    //% "Feeds"
    title: !folder ? qsTrId("id-feeds") : folder.name

    objectName: "foldersPage"

    property Folder folder: null

    contextualActions: [
        Kirigami.Action {
            iconName: cc.sortOrder === Qt.AscendingOrder ? "view-sort-ascending" : "view-sort-descending"
            text: cc.sortOrder === Qt.AscendingOrder
                    //% "Sort ascending
                  ? qsTrId("fuoten-sort-ascending-label")
                    //% "Sort descending"
                  : qsTrId("fuoten-sort-descending-label")
            onTriggered: if (cc.sortOrder === Qt.AscendingOrder) {
                cc.sortOrder = Qt.DescendingOrder
            } else {
                cc.sortOrder = Qt.AscendingOrder
            }
        },
        Kirigami.Action {
            iconName: "visibility"
            //% "Hide read"
            text: qsTrId("fuoten-hide-read-label")
            checkable: true
            checked: cc.hideRead
            onTriggered: cc.hideRead = !cc.hideRead
        },
        Kirigami.Action {
            iconName: "go-parent-folder"
            //% "Sort by folder"
            text: qsTrId("fuoten-sortbyfolder-label")
            checkable: true
            enabled: !folder
            checked: cc.showFolderSections
            onTriggered: cc.showFolderSections = !cc.showFolderSections
        }

    ]

    ListView {
        model: FeedListFilterModel {
            storage: localstorage
            parentId: folder ? folder.id : -1
            sortOrder: cc.sortOrder
            hideRead: cc.hideRead
            sortByFolder: cc.showFolderSections
            Component.onCompleted: load()
        }

        ContextConfig {
            id: cc
            contextType: !folder ? FuotenApp.StartPage : FuotenApp.Feeds
            contextId: !folder ? -1 : folder.id
        }

        section {
            property: 'display.folderName'
            delegate: cc.showFolderSections && !folder ? secHeader : null
        }

        delegate: Kirigami.SwipeListItem {

            RowLayout {
                anchors { left: parent.left; leftMargin: Kirigami.Units.largeSpacing; right: parent.right; rightMargin: Kirigami.Units.largeSpacing * 1.5 }

                CachedImage {
                    Layout.preferredWidth: Kirigami.Units.iconSizes.smallMedium
                    sourceSize.height: Kirigami.Units.iconSizes.smallMedium
                    sourceUrl: model.display.faviconLink
                }

                Kirigami.Heading {
                    Layout.fillWidth: true
                    level: 2
                    text: model.display.title
                }

                CountBubble {
                    value: model.display.unreadCount
                }
            }

            actions: [
                Kirigami.Action {
                    iconName: "edit-delete"
                    //% "Delete feed"
                    tooltip: qsTrId("fuoten-delete-feed")
                }

            ]
        }

        Component {
            id: secHeader
            Kirigami.Heading {
                anchors { left: parent.left; leftMargin: Kirigami.Units.largeSpacing }
                level: 1
                text: section
                visible: text != ""
            }
        }
    }
}
