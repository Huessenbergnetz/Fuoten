/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2

/*
  Types:
  0 - New
  1 - Improved/Enhanced
  2 - Fixed
  3 - Note
*/

ListModel {
    ListElement {
        version: "0.7.1"
        date: 1606394206000
        entries: [
                ListElement { type: 2; issue: "63"; description: "update to libfuoten 0.8.2 to handle changes in News App API data types" }
        ]
    }

    ListElement {
        version: "0.7.0"
        date: 1602946026000
        entries: [
            ListElement { type: 3; issue: ""; description: "seventh preview release" },
            ListElement { type: 0; issue: "57"; description: "support for Nextcloud login flow" },
            ListElement { type: 0; issue: "60"; description: "account deletion support" },
            ListElement { type: 0; issue: "61"; description: "remote wipe support" },
            ListElement { type: 1; issue: "47"; description: "option for navigating back after marking feed/folder as read" },
            ListElement { type: 1; issue: "56"; description: "better sharing dialog" },
            ListElement { type: 1; issue: ""; description: "translation updates" }
        ]
    }

    ListElement {
        version: "0.6.6"
        date: 1557152996000
        entries: [
            ListElement { type: 3; issue: ""; description: "maintenance update for sixth preview release" },
            ListElement { type: 2; issue: "52"; description: "article list is not updated" },
            ListElement { type: 1; issue: ""; description: "translation updates" }
        ]
    }

    ListElement {
        version: "0.6.5"
        date: 1556634791000
        entries: [
            ListElement { type: 3; issue: ""; description: "maintenance update for sixth preview release" },
            ListElement { type: 0; issue: ""; description: "Chinese (China) translation by Rui Kon" },
            ListElement { type: 1; issue: "50"; description: "hide articles with future publish date" },
            ListElement { type: 1; issue: "51"; description: "only hide web view settings when open in external browser has been selected" }
        ]
    }

    ListElement {
        version: "0.6.4"
        date: 1543914907000
        entries: [
            ListElement { type: 3; issue: ""; description: "maintenance update for sixth preview release" },
            ListElement { type: 2; issue: "43"; description: "text that is too long destroys the layout" },
            ListElement { type: 2; issue: "45"; description: "renaming dialogs are not accepted on enter key" }
        ]
    }

    ListElement {
        version: "0.6.3"
        date: 1543256057000
        entries: [
            ListElement { type: 3; issue: ""; description: "maintenance update for sixth preview release" },
            ListElement { type: 0; issue: ""; description: "support compressed server responses (gzip and deflate)" },
            ListElement { type: 0; issue: ""; description: "French translation by Antoine Vacher" },
            ListElement { type: 1; issue: ""; description: "use icons fitting to the current screen pixel ratio" },
            ListElement { type: 1; issue: ""; description: "add an option to disable the push up menu on article page and hide it by default" },
            ListElement { type: 1; issue: ""; description: "set default request timeout to 300 seconds to improve reliablitiy for long running remote actions like marking many articles as read" },
            ListElement { type: 2; issue: "40"; description: "mark all items as read" }
        ]
    }

    ListElement {
        version: "0.6.2"
        date: 1509958611000
        entries: [
            ListElement { type: 3; issue: ""; description: "maintenance update for sixth preview release" },
            ListElement { type: 2; issue: "39"; description: "wrong association to startup wizard account page scroll decorator flickable property" }
        ]
    }

    ListElement {
        version: "0.6.1"
        date: 1509098965000
        entries: [
            ListElement { type: 3; issue: ""; description: "maintenance update for sixth preview release" },
            ListElement { type: 2; issue: "38"; description: "initial synchronization fails" },
            ListElement { type: 1; issue: ""; description: "add cover action to copy article link to clipboard" },
            ListElement { type: 1; issue: ""; description: "add share entry to article page push up menu" }
        ]
    }

    ListElement {
        version: "0.6.0"
        date: 1509057223000
        entries: [
            ListElement { type: 3; issue: ""; description: "sixth preview release" },
            ListElement { type: 0; issue: "36"; description: "use notifications to notify about succeeded tasks and occuring errors" },
            ListElement { type: 0; issue: "25"; description: "pseudo background updates that synchronize with the server activated by user interaction" },
            ListElement { type: 0; issue: ""; description: "share links with the SailfishOS sharing mechanism" },
            ListElement { type: 0; issue: ""; description: "settings for font sizes in internal view and internal web view" },
            ListElement { type: 1; issue: ""; description: "store the ownCloud/Nextcloud password AES256 encrypted" },
            ListElement { type: 1; issue: "21"; description: "navigation buttons for the internal web view" },
            ListElement { type: 1; issue: "31"; description: "show information about current article on the cover" },
            ListElement { type: 1; issue: ""; description: "user agent for the web view is now selectable of a per feed setting" },
            ListElement { type: 2; issue: "30"; description: "remorse item on feed entry in folder view destroys the layout" },
            ListElement { type: 2; issue: "37"; description: "long feed and folder names crash the application" }
        ]
    }

    ListElement {
        version: "0.5.2"
        date: 1489940080000
        entries: [
            ListElement { type: 3; issue: ""; description: "maintenance update for fifth preview release" },
            ListElement { type: 0; issue: ""; description: "Dutch translation by Nathan Follens" },
            ListElement { type: 2; issue: "35"; description: "scroll to top does not work on article view" }
        ]
    }

    ListElement {
        version: "0.5.1"
        date: 1485686712000
        entries: [
            ListElement { type: 3; issue: ""; description: "maintenance update for fifth preview release" },
            ListElement { type: 2; issue: "34"; description: "clicking a link in an article does nothing" },
            ListElement { type: 1; issue: ""; description: "translation updates" }
        ]
    }

    ListElement {
        version: "0.5.0"
        date: 1479845385000
        entries: [
            ListElement { type: 3; issue: ""; description: "fifth preview release" },
            ListElement { type: 0; issue: "22"; description: "cover information and cover action" },
            ListElement { type: 0; issue: "26"; description: "per feed storage cleanup" },
            ListElement { type: 0; issue: "28"; description: "notification and progress area" },
            ListElement { type: 1; issue: "33"; description: "feed icon" },
            ListElement { type: 1; issue: "29"; description: "request items after adding feed" },
            ListElement { type: 1; issue: ""; description: "translation updates" }
        ]
    }

    ListElement {
        version: "0.4.0"
        date: 1478792919000
        entries: [
            ListElement { type: 3; issue: ""; description: "fourth preview release" },
            ListElement { type: 0; issue: "24"; description: "show last sync time in pull down menu" },
            ListElement { type: 0; issue: "23"; description: "copy article url to clipboard menu entry" },
            ListElement { type: 0; issue: "15"; description: "article content display" },
            ListElement { type: 0; issue: "19"; description: "context setting for article opening" },
            ListElement { type: 0; issue: "18"; description: "mark all articles as read" },
            ListElement { type: 0; issue: "20"; description: "optional article excerpt in article list" },
            ListElement { type: 0; issue: ""; description: "Swedish translation by Åke Engelbrektson" }
        ]
    }

    ListElement {
        version: "0.3.0"
        date: 1478558957000
        entries: [
            ListElement { type: 3; issue: ""; description: "third preview release" },
            ListElement { type: 0; issue: "14"; description: "article list" },
            ListElement { type: 0; issue: "16"; description: "mark articles as read/unread" },
            ListElement { type: 0; issue: "17"; description: "star/unstar articles" },
            ListElement { type: 0; issue: ""; description: "partly Danish translation by Peter Jespersen" }
        ]
    }

    ListElement {
        version: "0.2.0"
        date: 1478089348000
        entries: [
            ListElement { type: 3; issue: ""; description: "second preview release"},
            ListElement { type: 0; issue: "6"; description: "feeds list view" },
            ListElement { type: 0; issue: "8"; description: "feeds list sorting" },
            ListElement { type: 0; issue: "12"; description: "mark feeds as read" },
            ListElement { type: 0; issue: "11"; description: "move feeds" },
            ListElement { type: 0; issue: "9"; description: "add feeds" },
            ListElement { type: 0; issue: "13"; description: "delete feeds" },
            ListElement { type: 0; issue: "10"; description: "rename feeds" }
        ]
    }

    ListElement {
        version: "0.1.0"
        date: 1477643507000
        entries: [
            ListElement { type: 3; issue: ""; description: "first preview release"},
            ListElement { type: 0; issue: "1"; description: "folder list view" },
            ListElement { type: 0; issue: "7"; description: "folder list sorting"},
            ListElement { type: 0; issue: "3"; description: "delete folders"},
            ListElement { type: 0; issue: "2"; description: "create folders"},
            ListElement { type: 0; issue: "4"; description: "rename folders"},
            ListElement { type: 0; issue: "5"; description: "mark folder as read"}
        ]
    }
}

