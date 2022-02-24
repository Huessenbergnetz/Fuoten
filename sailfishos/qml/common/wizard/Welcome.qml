/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
import "../models"

Dialog {
    id: welcomeDialog

    backNavigation: false
    acceptDestination: Qt.resolvedUrl("LoginFlowSetup.qml")
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

