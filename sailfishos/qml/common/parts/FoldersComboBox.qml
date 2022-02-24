/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.fuoten.models 1.0

ComboBox {

    //: Label for the folder selection combo box
    //% "Folder"
    label: qsTrId("fuoten-folder")

    menu: ContextMenu {
        MenuItem {
            //: Selectable entry in the folder selection combo box
            //% "-- No folder --"
            text: qsTrId("fuoten-no-folder")
            readonly property int folderId: 0
        }

        Repeater {
            model: FolderListFilterModel {
                storage: localstorage
                Component.onCompleted: load()
            }

            MenuItem {
                text: display.name
                readonly property int folderId: display.id
            }
        }
    }
}
