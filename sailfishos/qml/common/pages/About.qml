/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import de.huessenbergnetz.hbnsc 1.0
import harbour.fuoten 1.0
import "../models"

AboutPage {
    appTitle: "Fuoten"
    //: Description on the about page
    //% "Client for the ownCloud/Nextcloud News App."
    appDescription: qsTrId("about-desc")
    appHomepage: "https://github.com/Huessenbergnetz/Fuoten"
    appCopyrightYearFrom: "2016"
    appCopyrightYearTo: "2022"
    appCopyrightHolder: "Matthias Fehring"
    appLicense: "GNU General Public License, Version 3"
    appLicenseFile: "GPLv3.qml"

    changelogModel: ChangelogModel {}
    bugTrackerBaseUrl: "https://github.com/Huessenbergnetz/Fuoten/issues/"

    contributorsModel: ContributorsModel {}

    contactCompany: "Hüssenbergnetz"
    contactName: "Matthias Fehring"
    contactStreet: "Zum Südholz"
    contactHouseNo: "8"
    contactZIP: "34439"
    contactCity: "Willebadessen-Eissen"
    //% "Germany"
    contactCountry: qsTrId("id-germany")
    contactEmail: Qt.atob("a29udGFrdEBodWVzc2VuYmVyZ25ldHouZGU=")
    contactWebsite: "www.huessenbergnetz.de"
    contactWebsiteLink: "https://www.huessenbergnetz.de/?pk_campaign=Sailfish-App-Fuoten&pk_kwd=AboutPage"

    bugUrl: "https://github.com/Huessenbergnetz/Fuoten/issues"
    translateUrl: "https://www.transifex.com/huessenbergnetz/fuoten"

    licensesModel: LicensesModel {}

    privacyPolicyQmlFile: Qt.resolvedUrl("PrivacyPolicy.qml")
}

