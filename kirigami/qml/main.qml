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
import org.kde.kirigami 1.0 as Kirigami
import buschtrommel.fuoten.models 1.0
import "navigation.js" as Navigation

Kirigami.ApplicationWindow {
    id: fuotenWindow
    globalDrawer: Kirigami.GlobalDrawer {
        title: "Fuoten"
        titleIcon: "applications-graphics"
        actions: [
            Kirigami.Action {
                //% "%n unread article(s)"
                text: qsTrId("fuoten-unread-articles-with-count").arg(localstorage.totalUnread)
                iconName: "visibility"
            },

            Kirigami.Action {
                //% "%n starred article(s)"
                text: qsTrId("fuoten-starred-articles-with-count").arg(localstorage.stared)
                iconName: "love"
            },

            Kirigami.Action {
                //% "Folders"
                text: qsTrId("id-folders")
                iconName: "document-open-folder"
                onTriggered: Navigation.openFolders()
            },

            Kirigami.Action {
                //% "Feeds"
                text: qsTrId("id-feeds")
                iconName: "feed-subscribe"
                onTriggered: Navigation.openFeeds()
            },

            Kirigami.Action {
                //% "Add feed or folder"
                text: qsTrId("fuoten-add-feeds-folders")
                iconName: "list-add"

                Kirigami.Action {
                    //% "Create folder"
                    text: qsTrId("fuoten-create-folder")
                    iconName: "folder-new"
                }

                Kirigami.Action {
                    //% "Add feed"
                    text: qsTrId("fuoten-add-feed")
                    iconName: "kt-add-feeds"
                }
            }


//            Kirigami.Action {
//                //% "Synchronize"
//                text: qsTrId("fuoten-synchronize")
//                iconName: "view-refresh"
//            },

//            Kirigami.Action {
//                //% "Settings"
//                text: qsTrId("id-settings")
//                iconName: "configure"
//                checked: fuotenWindow.pageStack.currentItem.objectName === "settingsPage"
//                onTriggered: !checked ? Navigation.openSettings() : ""
//            }

        ]


        ColumnLayout {
            spacing: 0
            Layout.fillWidth: true

            Kirigami.Separator {
                Layout.fillWidth: true
            }

            Kirigami.BasicListItem {
                icon: "view-refresh"
                //% "Synchronize"
                label: qsTrId("fuoten-synchronize")
                separatorVisible: false
                onClicked: synchronizer.sync()
                enabled: !synchronizer.inOperation
            }

            Kirigami.BasicListItem {
                icon: "configure"
                //% "Settings"
                label: qsTrId("id-settings")
                separatorVisible: false
                checked: fuotenWindow.pageStack.currentItem.objectName === "settingsPage"
                onClicked: !checked ? Navigation.openSettings() : ""
            }
        }
    }
    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }
    pageStack.initialPage: mainPageComponent
    Component {
        id: mainPageComponent
        Kirigami.ScrollablePage {
            title: "Hello"
//            actions {
//                main: Kirigami.Action {
//                    iconName: sheet.opened ? "dialog-cancel" : "document-edit"
//                    onTriggered: {
//                        print("Action button in buttons page clicked");
//                        sheet.opened = !sheet.opened
//                    }
//                }
//                left: Kirigami.Action {
//                    iconName: "go-previous"
//                    onTriggered: {
//                        print("Left action triggered")
//                    }
//                }
//                right: Kirigami.Action {
//                    iconName: "go-next"
//                    onTriggered: {
//                        print("Right action triggered")
//                    }
//                }
//                contextualActions: [
//                    Kirigami.Action {
//                        text:"Action for buttons"
//                        iconName: "bookmarks"
//                        onTriggered: print("Action 1 clicked")
//                    },
//                    Kirigami.Action {
//                        text:"Action 2"
//                        iconName: "folder"
//                        enabled: false
//                    },
//                    Kirigami.Action {
//                        text: "Action for Sheet"
//                        visible: sheet.opened
//                    }
//                ]
//            }
            //Page contents...
        }
    }
}
