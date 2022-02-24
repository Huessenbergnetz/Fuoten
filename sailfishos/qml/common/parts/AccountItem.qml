/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
import harbour.fuoten 1.0
import harbour.fuoten.api 1.0

BackgroundItem {
    id: accountArea

    contentHeight: (busyInd.visible || accountErr.visible || invalidAccountText.visible || accRow.visible) ? Theme.itemSizeHuge : 0

    enabled: !inOperation

    function check() {
        userAvatar.check()
        getServerStatus.execute()
        getStatus.execute()
    }

    readonly property bool inOperation: getServerStatus.inOperation || getStatus.inOperation
    readonly property bool hasError: getServerStatus.error || getStatus.error

    GetServerStatus {
        id: getServerStatus
    }

    GetStatus {
        id: getStatus
    }

    UserAvatar {
        id: userAvatar
    }

    onClicked: {
        var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/AccountSetup.qml"))
        dialog.accepted.connect(function() {
            accountArea.check()
        })
    }

    BusyIndicator {
        id: busyInd
        visible: accountArea.inOperation
        running: accountArea.inOperation
        size: BusyIndicatorSize.Medium
        anchors.centerIn: parent
    }

    ErrorItem {
        id: accountErr
        anchors {left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
        visible: !accountArea.inOperation && accountArea.hasError
        error: getServerStatus.error ? getServerStatus.error : getStatus.error ? getStatus.error : null
        highlighted: accountArea.highlighted
    }

    Text {
        id: invalidAccountText
        anchors {left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
        font.pixelSize: Theme.fontSizeSmall
        color: accountArea.highlighted ? Theme.highlightColor : Theme.primaryColor
        //% "Your account is not or improperly configured. Click here to configure your account."
        text: qsTrId("id-settings-account-invalid")
        wrapMode: Text.WordWrap
        visible: !accountArea.inOperation && !config.isAccountValid && !accountArea.hasError
    }

    RowLayout {
        id: accRow
        anchors {left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
        spacing: Theme.paddingSmall
        visible: config.isAccountValid && !accountArea.inOperation && !accountArea.hasError

        Image {
            id: avatarImage
            source: userAvatar.avatarUrl
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.preferredWidth: Theme.iconSizeLarge
            fillMode: Image.PreserveAspectFit
        }

        Column {
            Layout.fillWidth: true
            spacing: Theme.paddingSmall

            Label {
                id: usernameString
                text: config.username
                font.pixelSize: Theme.fontSizeSmall
                color: accountArea.highlighted ? Theme.highlightColor : Theme.primaryColor
                textFormat: Text.PlainText
                width: parent.width
                truncationMode: TruncationMode.Fade
                maximumLineCount: 1
            }

            Label {
                width: parent.width
                font.pixelSize: Theme.fontSizeSmall
                color: accountArea.highlighted ? Theme.highlightColor : Theme.primaryColor
                text: String("%1%2").arg(config.host).arg(config.installPath)
                truncationMode: TruncationMode.Fade
                maximumLineCount: 1
            }

            RowLayout {
                id: statusRow
                spacing: Theme.paddingSmall

                Item {
                    Layout.fillWidth: true
                    Layout.preferredWidth: versionString.implicitWidth
                    height: versionString.height
                    Text {
                        id: versionString
                        font.pixelSize: Theme.fontSizeSmall
                        color: accountArea.highlighted ? Theme.highlightColor : Theme.primaryColor
                        //: %1 is the News App version number
                        //% "Version %1"
                        text: qsTrId("id-settings-version").arg(getStatus.versionString)
                        textFormat: Text.PlainText
                        width: parent.width
                    }
                }
                Image {
                    id: statusIcon
                    property color color: accountArea.highlighted ? Theme.highlightColor : Theme.primaryColor
                    source: config.useSSL && !config.ignoreSSLErrors ? "image://theme/icon-s-installed?" + color : "image://theme/icon-s-high-importance?" + color
                    Layout.preferredWidth: Theme.iconSizeSmall
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    fillMode: Image.PreserveAspectFit
                }
                Item {
                    Layout.preferredWidth: accRow.width - versionString.implicitWidth - avatarImage.width - statusIcon.implicitWidth - accRow.spacing - (2* statusRow.spacing)
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    height: statusString.height
                    Label {
                        id: statusString
                        width: parent.width
                        font.pixelSize: Theme.fontSizeSmall
                        color: accountArea.highlighted ? Theme.highlightColor : Theme.primaryColor
                        text: config.useSSL && !config.ignoreSSLErrors
                              //% "HTTPS enabled"
                              ? qsTrId("id-settings-ssl-enabled")
                              : config.useSSL && config.ignoreSSLErrors
                                //% "Ignoring errors"
                                ? qsTrId("id-settings-ignore-ssl-errors")
                                  //% "HTTPS disabled"
                                : qsTrId("id-settings-ssl-disabled")
                        truncationMode: TruncationMode.Fade
                        textFormat: Text.PlainText
                    }
                }
            }

            Row {
                width: parent.width
                spacing: Theme.paddingSmall

                Label {
                    id: productName
                    width: parent.width - parent.spacing - ncVersion.implicitWidth
                    font.pixelSize: Theme.fontSizeSmall
                    color: accountArea.highlighted ? Theme.highlightColor : Theme.primaryColor
                    textFormat: Text.PlainText
                    truncationMode: TruncationMode.Fade
                    text: getServerStatus.productname
                }

                Label {
                    id: ncVersion
                    font.pixelSize: Theme.fontSizeSmall
                    color: accountArea.highlighted ? Theme.highlightColor : Theme.primaryColor
                    textFormat: Text.PlainText
                    text: getServerStatus.versionString
                }
            }
        }
    }
}

