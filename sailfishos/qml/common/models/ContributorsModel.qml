/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2

ListModel {
    id: contModel
    ListElement {
        name: "Matthias Fehring (Buschmann)"
        role: ""
        section: ""
        image: "buschmann.png"
        website: "https://www.buschmann23.de/?pk_campaign=Sailfish-App-Fuoten&pk_kwd=ContributorsList"
        twitter: "buschmann23"
        github: "buschmann23"
    }

    ListElement {
        name: "Åke Engelbrektson"
        role: ""
        section: ""
        website: "https://svenskasprakfiler.se"
    }

    ListElement {
        name: "Antoine Vacher (Tigre-Bleu)"
        role: ""
        section: ""
        image: "tigre-bleu.png"
        website: "https://www.tigre-bleu.net/"
        twitter: "tigre_bleu"
        github: "tigre-bleu"
    }

    ListElement {
        name: "Nathan Follens"
        role: ""
        section: ""
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

    ListElement {
        name: "Rui Kon"
        role: ""
        section: ""
    }

    Component.onCompleted: {
        //: contributor role
        //% "Main developer, Fuoten creator"
        contModel.get(0).role = qsTrId("fuoten-author-role")
        //: the section a contributor belongs to
        //% "Author"
        contModel.get(0).section = qsTrId("fuoten-author-section")

        //: contributor role
        //% "Swedish translation"
        contModel.get(1).role = qsTrId("fuoten-eson-role")
        //: the section a contributor belongs to
        //% "Thanks to"
        contModel.get(1).section = qsTrId("fuotn-thanks-to-section")

        //: contributor role
        //% "French translation"
        contModel.get(2).role = qsTrId("fuoten-tigrebleu-role")
        contModel.get(2).section = qsTrId("fuotn-thanks-to-section")

        //: contributor role
        //% "Dutch translation"
        contModel.get(3).role = qsTrId("fuoten-nathanfollens-role")
        contModel.get(3).section = qsTrId("fuotn-thanks-to-section")

        //: contributor role
        //% "Danish translation"
        contModel.get(4).role = qsTrId("fuoten-flywheeldk-role")
        contModel.get(4).section = qsTrId("fuotn-thanks-to-section")

        //: contributor role
        //% "Chinese (China) translation"
        contModel.get(5).role = qsTrId("fuoten-ruikon-role")
        contModel.get(5).section = qsTrId("fuotn-thanks-to-section")
    }
}

