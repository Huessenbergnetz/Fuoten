/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/qml/phone/pages/MainPage.qml
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
        text: "<a href='https://www.buschmann23.de/?pk_campaign=Sailfish-App-Fuoten&pk_kwd=PrivacyPolicy'>https://www.buschmann23.de</a>"
    }

    Component.onCompleted: {
        //% "What information does Fuoten collect?"
        ppmodel.get(0).text = qsTrId("fuoten-privacy-0")
        //% "In order to log in to your ownCloud/Nextcloud server, Fuoten collects the server address, your user name and your password. These information is only saved on your device and is not given to the author of Fuoten or other third parties."
        ppmodel.get(1).text = qsTrId("fuoten-privacy-1")
        //% "What does Fuoten use your information for?"
        ppmodel.get(2).text = qsTrId("fuoten-privacy-2")
        //% "Fuoten needs your log in data to connect to your ownCloud/Nextcloud server. Your login data is used to grant Fuoten access to the News App API to perform the tasks of synchronizing your ownCloud/Nextcloud News App data with your device."
        ppmodel.get(3).text = qsTrId("fuoten-privacy-3")
        //% "Does ocNews disclose any information to outside parties?"
        ppmodel.get(4).text = qsTrId("fuoten-privacy-4")
        //% "Fuoten only uses your data for the login process into the server that you configure in the settings. Fuoten does not send these data to the author of Fuoten or other third parties. For questions concerning the privacy on the server side, you have to contact your ownCloud/Nextcloud service provider."
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

