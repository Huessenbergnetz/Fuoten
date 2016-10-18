/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/qml/common/pages/About.qml
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
import harbour.fuoten.btsc 1.0
import "../models"

AboutPage {
    //% "About"
    pageTitle: qsTrId("id-about")

    appTitle: "Fuoten"
    //% "Client for the ownCloud/Nextcloud News App."
    appDescription: qsTrId("about-desc")
    appHomepage: "https://www.buschmann23.de/entwicklung/anwendungen/gibrievida/"
    appCopyrightYear: "2016"
    appCopyrightHolder: "Buschtrommel / Matthias Fehring"
    appLicense: "GNU General Public License, Version 3"
    appLicenseFile: "GPLv3.qml"

    changelogModel: ChangelogModel {}
    bugTrackerBaseUrl: "https://github.com/Buschtrommel/Fuoten/issues/"

    contributorsModel: ContributorsModel {}
    contributorsAvatarBasePath: "/usr/share/harbour-fuoten/images/contributors"
    contributorsPlaceholderPath: "/usr/share/harbour-fuoten/images/contributors/placeholder.png"

    contactCompany: "Buschtrommel"
    contactName: "Matthias Fehring"
    contactStreet: "Engelhardstraße"
    contactHouseNo: "32"
    contactZIP: "81369"
    //% "Munich"
    contactCity: qsTrId("id-munich")
    //% "Germany"
    contactCountry: qsTrId("id-germany")
    contactEmail: "kontakt@buschmann23.de"
    contactWebsite: "www.buschmann23.de"

    bugUrl: "https://github.com/Buschtrommel/Fuoten/issues"
    translateUrl: "https://www.transifex.com/buschtrommel/fuoten"

    licensesModel: LicensesModel {}

    paypalOrganization: "Buschtrommel"
    paypalItem: "Fuoten"
    paypalEmail: "kontakt@buschmann23.de"
//    paypalMessage: donation.message
//    paypalLabel: donation.label
//    paypalDescription: donation.description
}

