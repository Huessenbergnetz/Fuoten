/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2019 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * common/qml/common/models/PrivacyPolicyModel.qml
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

