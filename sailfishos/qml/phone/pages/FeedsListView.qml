/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/qml/phone/pages/FeedListView.qml
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
//import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
import harbour.fuoten 1.0
import harbour.fuoten.models 1.0
import harbour.fuoten.items 1.0
import "../../common/parts"

BaseListView {
    id: feedListView

    contextType: !folder ? FuotenApp.StartPage : FuotenApp.Feeds
    inOperation: feedListModel.inOperation

    //: Placeholder text on the article list view if no feeds have been found
    //% "No feeds found"
    noContentText: qsTrId("fuoten-no-feeds-found")

    header: ListPageHeader {
        id: feedsListHeader
        page: feedListView.page
        searchVisible: feedListView.searchVisible
        folders: false
        folder: feedListView.folder
        onSearchTextChanged: feedListView.searchString = searchText
        onAllArticlesClicked: folder ? pageStack.push(Qt.resolvedUrl("ArticlesListPage.qml"), {context: FuotenApp.FolderItems, folder: folder}) : pageStack.push(Qt.resolvedUrl("ArticlesListPage.qml"), {context: FuotenApp.AllItems})
        onStarredItemsClicked: pageStack.push(Qt.resolvedUrl("ArticlesListPage.qml"), {context: FuotenApp.StarredItems})
    }

    model: FeedListFilterModel {
        id: feedListModel
        parentId: feedListView.folder ? feedListView.folder.id : -1
        storage: localstorage
        sortingRole: feedListView.cc.sorting
        search: feedListView.searchString
        hideRead: feedListView.cc.hideRead
        sortOrder: feedListView.cc.sortOrder
        respectPinned: feedListView.cc.respectPinned
        sortByFolder: feedListView.cc.showFolderSections
        Component.onCompleted: load(config.language)
    }

    section {
        property: 'display.folderName'
        delegate: feedListView.cc.showFolderSections && !folder ? secHeader : null
    }

    delegate: FeedListDelegate { searchString: feedListView.searchString }

    Component {
        id: secHeader
        SectionHeader {
            text: section
            visible: text != ""
            height: Theme.itemSizeExtraSmall
        }
    }
}
