/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/qml/common/models/LicensesModel.qml
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

ListModel {
    id: licensesModel

    ListElement {
        name: "libfuoten"
        author: "Buschtrommel/Matthias Fehring"
        version: "0.0.1"
        license: "GNU Lesser General Public License, Version 3"
        licenseFile: "LGPLv3.qml"
        website: "https://github.com/Buschtrommel/libfuoten"
        description: ""
    }

    ListElement {
        name: "BT SFOS Components"
        author: "Buschtrommel/Matthias Fehring"
        version: "1.0.0"
        license: "Modified BSD License"
        licenseFile: "BSD-3.qml"
        website: "https://github.com/Buschtrommel/BT_SFOS_Components"
        description: ""
    }

    ListElement {
        name: "Fuoten Translations"
        author: "Fuoten Translators"
        license: "Creative Commons Attribution 4.0 International Public License"
        licenseFile: "CC-BY-4_0.qml"
        website: "https://www.transifex.com/buschtrommel/fuoten"
    }

    Component.onCompleted: {
        //% "Libfuoten is a Qt based library that provides access the ownCloud/Nextcloud News App API."
        licensesModel.get(0).description = qsTrId("fuoten-libfuoten-desc")
        //% "BT SFOS Components are a set of QML components for Sailfish OS."
        licensesModel.get(1).description = qsTrId("btsfoscmops-desc")
    }
}

