/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.fuoten.api 1.0
import "../parts"

Dialog {
    id: loginFlowPolling
    canAccept: config.isAccountValid && !account.inOperation
    backNavigation: !config.isAccountValid && !account.inOperation
    acceptDestination: Screen.sizeCategory >= Screen.Large
                           ? Qt.resolvedUrl("../../tablet/pages/MainPage.qml")
                           : Qt.resolvedUrl("../../phone/pages/MainPage.qml")
    acceptDestinationAction: PageStackAction.Replace
    acceptDestinationReplaceTarget: null

    property url loginUrl: ""

    LoginFlowV2 {
        id: loginFlowV2
        onGotLoginUrl: { loginFlowPolling.loginUrl = url; Qt.openUrlExternally(url) }
        onSucceeded: account.check()
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            loginFlowV2.execute()
        }
    }

    onAccepted: {
        config.setCurrentVersion()
        synchronizer.sync()
    }

    SilicaFlickable {
        id: loginFlowPollingFlick
        anchors.fill: parent
        contentHeight: loginFlowPollingHeader.height + loginFlowPollingCol.height

        VerticalScrollDecorator { flickable: loginFlowPollingFlick; page: loginFlowPolling }

        PullDownMenu {
            flickable: loginFlowPollingFlick
            enabled: !config.isAccountValid
            visible: !config.isAccountValid
            MenuItem {
                //: Pull down menu entry to reopen the external authorization browser window
                //% "Open in browser"
                text: qsTrId("fuoten-loginflow-open-loginurl")
                enabled: loginFlowV2.inOperation && loginFlowPolling.loginUrl != ""
                onClicked: Qt.openUrlExternally(loginFlowPolling.loginUrl)
            }
            MenuItem {
                //: Pull down menu entry to retry the authorization with external browser
                //% "Retry"
                text: qsTrId("fuoten-loginflow-polling-retry")
                enabled: !loginFlowV2.inOperation
                onClicked: { loginFlowPolling.loginUrl = ""; loginFlowV2.execute() }
            }
        }

        DialogHeader {
            id: loginFlowPollingHeader
            title: config.isAccountValid
                     //: Dialog title in the login flow if authorization has succeeded
                     //% "Authorization succeeded"
                   ? qsTrId("fuoten-loginflow-polling-title-success")
                     //: Dialog title in the login flow while waiting for authoriaztion
                     //% "Waiting for authorization"
                   : qsTrId("fuoten-loginflow-polling-title-waiting")
            dialog: loginFlowPolling
            flickable: loginFlowPollingFlick
            //: Text for finishing the first start configuration wizard
            //% "Finish"
            defaultAcceptText: qsTrId("fuoten-wizard-finish")
        }

        Column {
            id: loginFlowPollingCol
            anchors { left: parent.left; right: parent.right; top: loginFlowPollingHeader.bottom }

            BusyIndicator {
                id: loginFlowPollingBusy
                anchors.horizontalCenter: parent.horizontalCenter
                size: BusyIndicatorSize.Large
                running: visible
                visible: loginFlowV2.inOperation
            }

            Text {
                anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin }
                color: Theme.secondaryColor
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeSmall
                visible: loginFlowV2.inOperation
                //% "%n second(s) left"
                text: qsTrId("fuoten-loginflow-seconds-left", loginFlowV2.secondsLeft)
            }

            AccountItem {
                id: account
                anchors { left: parent.left; right: parent.right }
                enabled: false
                visible: !loginFlowV2.inOperation
            }

            ErrorItem {
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                error: loginFlowV2.error
                visible: loginFlowV2.error && !loginFlowV2.inOperation
            }
        }
    }
}
