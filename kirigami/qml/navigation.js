/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/src/main.cpp
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


function clearStack() {
    fuotenWindow.currentTopLevel = null
    fuotenWindow.stack.clear()
}

function openSettings() {
    fuotenWindow.pageStack.clear()
    fuotenWindow.pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
}

function openFolders() {
    fuotenWindow.pageStack.clear()
    fuotenWindow.pageStack.push(Qt.resolvedUrl("FoldersPage.qml"))
}


function openFeeds() {
    fuotenWindow.pageStack.clear()
    fuotenWindow.pageStack.push(Qt.resolvedUrl("FeedsPage.qml"))
}
