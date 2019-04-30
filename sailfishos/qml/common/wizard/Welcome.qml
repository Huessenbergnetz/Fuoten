/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2019 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/qml/common/wizard/Welcome.qml
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
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
import "../models"

Dialog {
    id: welcomeDialog

    backNavigation: false
    acceptDestination: Qt.resolvedUrl("Account.qml")
    acceptDestinationAction: PageStackAction.Replace

    SilicaFlickable {
        id: welcomeFlick
        anchors.fill: parent
        contentHeight: welcomeCol.height + dHeader.height

        VerticalScrollDecorator { flickable: welcomeFlick; page: welcomeDialog }

        DialogHeader {
            id: dHeader;
            //: Dialog header on the first start wizard
            //% "Welcome to Fuoten"
            title: qsTrId("fuoten-welcome");
            dialog: welcomeDialog;
            flickable: welcomeFlick
        }

        Column {
            id: welcomeCol
            anchors { left: parent.left; right: parent.right; top: dHeader.bottom }
            spacing: Theme.paddingMedium

            Text {
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                //: Note about acceptance of the privacy policy on the first start wizard
                //% "By using Fuoten, you consent to the following privacy policy."
                text: qsTrId("fuoten-welcome-privacy-consent")
                wrapMode: Text.WordWrap
            }

            Item {
                width: parent.width
                height: Theme.paddingLarge
            }

            Label {
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeLarge
                truncationMode: TruncationMode.Fade
                text: qsTrId("fuoten-privacy-policy")
            }

            Repeater {
                id: licenseRep
                model: PrivacyPolicyModel {}

                delegate: Text {
                    anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                    color: model.header ? Theme.highlightColor : Theme.primaryColor
                    font.pixelSize: model.header ? Theme.fontSizeMedium : Theme.fontSizeSmall
                    textFormat: model.format ? model.format : Text.PlainText
                    onLinkActivated: Qt.openUrlExternally(link)
                    text: model.text
                    wrapMode: Text.WordWrap
                    linkColor: Theme.highlightColor
                }
            }

            Item {
                width: parent.width
                height: Theme.paddingLarge
            }
        }
    }
}

