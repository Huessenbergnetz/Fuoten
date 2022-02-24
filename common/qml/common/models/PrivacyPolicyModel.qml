/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2

ListModel {
    id: ppmodel

    ListElement {
        header: true
        text: ""
    }

    ListElement {
        text: ""
    }

    ListElement {
        header: true
        text: ""
    }

    ListElement {
        text: ""
    }

    ListElement {
        header: true
        text: ""
    }

    ListElement {
        text: ""
    }

    ListElement {
        header: true
        text: ""
    }

    ListElement {
        text: ""
    }

    ListElement {
        header: true
        text: ""
    }

    ListElement {
        text: ""
    }

    ListElement {
        header: true
        text: ""
    }

    ListElement {
        format: Text.StyledText
        text: "<a href='https://www.huessenbergnetz.de/?pk_campaign=Sailfish-App-Fuoten&pk_kwd=PrivacyPolicy'>https://www.huessenbergnetz.de</a>"
    }

    Component.onCompleted: {
        //% "What information does Fuoten collect?"
        ppmodel.get(0).text = qsTrId("fuoten-privacy-0")
        //% "To log in to your ownCloud/Nextcloud server, Fuoten requires your server address, username and password. This data will only be stored on your device and will not be passed on to the author of Fuoten or any other third party other than the operator of your server."
        ppmodel.get(1).text = qsTrId("fuoten-privacy-1")
        //% "What does Fuoten use your information for?"
        ppmodel.get(2).text = qsTrId("fuoten-privacy-2")
        //% "Fuoten needs your log in data to connect to your ownCloud/Nextcloud server. Your login data is used to grant Fuoten access to the News App API to perform the tasks of synchronizing your ownCloud/Nextcloud News App data with your device."
        ppmodel.get(3).text = qsTrId("fuoten-privacy-3")
        //% "Does Fuoten disclose any information to outside parties?"
        ppmodel.get(4).text = qsTrId("fuoten-privacy-4")
        //% "Fuoten uses your data only for the login process on the server you have set in the settings. Fuoten does not send this data to the author of Fuoten or to other third parties - except to the operator of your server. Questions regarding data protection on the server side should be directed to the operator of your server."
        ppmodel.get(5).text = qsTrId("fuoten-privacy-5")
        //% "Third party links"
        ppmodel.get(6).text = qsTrId("fuoten-privacy-6")
        //% "Fuoten is a cloud based RSS/Atom feed reader. The links to websites directly included in the application have been checked for legality. Showing feed content and/or opening feed link addresses is in the user's responsibility. For questions concerning the privacy of the used feed content and/or website content you have to contact the feed provider."
        ppmodel.get(7).text = qsTrId("fuoten-privacy-7")
        //% "Your consent"
        ppmodel.get(8).text = qsTrId("fuoten-privacy-8")
        //% "By using Fuoten, you consent to this privacy policy."
        ppmodel.get(9).text = qsTrId("fuoten-privacy-9")
        //% "Contact"
        ppmodel.get(10).text = qsTrId("fuoten-privacy-10")
    }
}

