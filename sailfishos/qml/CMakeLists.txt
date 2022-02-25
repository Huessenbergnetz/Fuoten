# SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

set(QMLSRCS
    harbour-fuoten.qml
    common/dialogs/AccountSetup.qml
    common/dialogs/CreateFeedDialog.qml
    common/dialogs/CreateFolderDialog.qml
    common/dialogs/MoveFeedDialog.qml
    common/dialogs/RenameFeedDialog.qml
    common/dialogs/RenameFolderDialog.qml
    common/models/ChangelogModel.qml
    common/models/ContributorsModel.qml
    common/pages/About.qml
    common/pages/ContextConfigPage.qml
    common/pages/PrivacyPolicy.qml
    common/pages/Settings.qml
    common/pages/Sharing.qml
    common/pages/WipeManager.qml
    common/parts/AccountItem.qml
    common/parts/CountBubble.qml
    common/parts/ErrorItem.qml
    common/parts/FoldersComboBox.qml
    common/parts/FontSizeSlider.qml
    common/parts/ListPageHeader.qml
    common/parts/RescalingRichText.qml
    common/parts/SyncPanel.qml
    common/wizard/DeleteAccount.qml
    common/wizard/DeleteAccountFinish.qml
    common/wizard/LoginFlowCheck.qml
    common/wizard/LoginFlowPolling.qml
    common/wizard/LoginFlowSetup.qml
    common/wizard/LoginManual.qml
    common/wizard/LoginManualCheck.qml
    common/wizard/Welcome.qml
    phone/cover/CoverPage.qml
    phone/pages/ArticlePage.qml
    phone/pages/ArticlesListPage.qml
    phone/pages/ArticlesListView.qml
    phone/pages/BaseListView.qml
    phone/pages/FeedListDelegate.qml
    phone/pages/FeedsListPage.qml
    phone/pages/FeedsListView.qml
    phone/pages/FolderListView.qml
    phone/pages/MainPage.qml
    phone/pages/WebViewPage.qml
    tablet/cover/CoverPage.qml
    tablet/pages/MainPage.qml
)

add_custom_target(qmlfiles
    SOURCES ${QMLSRCS})

install(FILES harbour-fuoten.qml DESTINATION ${FUOTEN_QMLDIR})

install(DIRECTORY common phone tablet DESTINATION ${FUOTEN_QMLDIR})
