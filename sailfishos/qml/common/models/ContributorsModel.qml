/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/qml/common/models/ChangelogModel.qml
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
    id: contModel
    ListElement {
        name: "Matthias Fehring (Buschmann)"
        role: ""
        section: ""
        image: "buschmann.png"
        website: "http://www.buschmann23.de"
        twitter: "buschmann23"
        github: "buschmann23"
    }

    ListElement {
        name: "Peter Jespersen (Flywheeldk)"
        role: ""
        section: ""
        image: "flywheeldk.png"
        website: "http://illogical.dk/"
        twitter: "myflywheel"
        linkedin: "pjespersen"
        bitbucket: "flywheel"
    }

    Component.onCompleted: {
        //% "Main developer, Fuoten creator"
        contModel.get(0).role = qsTrId("fuoten-author-role")
        //% "Author"
        contModel.get(0).section = qsTrId("fuoten-author-section")

        //% "Danish translation"
        contModel.get(1).role = qsTrId("fuoten-flywheeldk-role")
        //% "Thanks to"
        contModel.get(1).section = qsTrId("fuotn-thanks-to-section")
    }
}

