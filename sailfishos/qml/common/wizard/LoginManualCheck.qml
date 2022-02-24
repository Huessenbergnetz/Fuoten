/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
import harbour.fuoten 1.0
import "../parts"

Dialog {
    id: loginManualCheck
    canAccept: config.isAccountValid && !account.inOperation && !account.error
    backNavigation: !config.isAccountValid
    acceptDestination: Screen.sizeCategory >= Screen.Large
                       ? Qt.resolvedUrl("../../tablet/pages/MainPage.qml")
                       : Qt.resolvedUrl("../../phone/pages/MainPage.qml")
    acceptDestinationAction: PageStackAction.Replace
    acceptDestinationReplaceTarget: null

    onAccepted: {
        config.setCurrentVersion()
        synchronizer.sync()
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            account.check()
        }
    }

    SilicaFlickable {
        id: loginManualCheckFlick
        anchors.fill: parent
        contentHeight: loginManualCheckHeader.height + account.height

        VerticalScrollDecorator { flickable: loginManualCheckFlick; page: loginManualCheck }

        PullDownMenu {
            flickable: loginManualCheckFlick
            MenuItem {
                text: qsTrId("fuoten-loginflow-polling-retry")
                enabled: !account.inOperation
                onClicked: account.check()
            }
        }

        DialogHeader {
            id: loginManualCheckHeader
            dialog: loginManualCheck
            flickable: loginManualCheckFlick
            defaultAcceptText: qsTrId("fuoten-wizard-finish")
            title: account.inOperation
                   //: Dialog title in the login flow while checking manual account setup
                   //% "Checking account"
                   ? qsTrId("fuoten-loginflow-manual-check-account")
                   : account.error
                     //: Dialog title in the login flow if manual account setup failed
                     //% "Setup failed"
                     ? qsTrId("fuoten-loginflow-manual-check-failed")
                     : config.isAccountValid
                       //: Dialog title in the login flow if manual account setup succeeded
                       //% "Setup finished"
                       ? qsTrId("fuoten-loginflow-manual-check-success")
                       : qsTrId("fuoten-loginflow-manual-check-account")
        }

        AccountItem {
            id: account
            anchors { left: parent.left; right: parent.right; top: loginManualCheckHeader.bottom }
            enabled: false
        }
    }
}
