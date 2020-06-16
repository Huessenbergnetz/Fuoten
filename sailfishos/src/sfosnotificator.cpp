/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
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

#include "sfosnotificator.h"
#include "sfosconfig.h"
#include "hbnsc.h"
#include <QVariant>
#include <QStringBuilder>
#include <QLocale>
#include <QDebug>
#include <notification.h>

SfosNotificator::SfosNotificator(SfosConfig *config, QObject *parent) :
    Fuoten::AbstractNotificator(parent), m_config(config)
{
    setAppName(QStringLiteral("Fuoten"));
    const QString _iconsDir = Hbnsc::getIconsDir({1.0,1.25,1.5,1.75,2.0});
    m_infoIcon = _iconsDir % QStringLiteral("icon-lock-fuoten-info.png");
    m_errorIcon = _iconsDir % QStringLiteral("icon-lock-fuoten-error.png");
    Q_ASSERT(m_config);
}

SfosNotificator::~SfosNotificator()
{

}


void SfosNotificator::notify(Fuoten::AbstractNotificator::Type type, QtMsgType severity, const QVariant &data) const
{
    if (m_config->notificationsEnabled() || (severity == QtFatalMsg)) {

        bool force = false;

        switch (type) {
        case FoldersRequested:
        case FeedsRequested:
        case ItemsRequested:
        case FolderCreated:
        case FeedCreated:
        case AuthorizationSucceeded:
        case AppPasswordRequested:
            force = true;
            break;
        case SyncComplete:
            return;
        default:
            break;
        }

        qDebug() << "Preparing new notification:" << type << severity << data;

        if (isEnabled() || force || (severity == QtFatalMsg)) {

            QString previewSummary;
            QString previewBody;
            QString summary;
            QString body;
            Notification::Urgency urgency = Notification::Low;
            QString icon = m_infoIcon;
            qint32 expireTimeout = -1;
            QString category;
            QString origin;

            switch(severity) {
            case QtCriticalMsg:
                urgency = Notification::Normal;
                icon = m_errorIcon;
                break;
            case QtFatalMsg:
                urgency = Notification::Critical;
                icon = m_errorIcon;
                break;
            default:
                break;
            }

            switch (type) {
            case GeneralError:
                expireTimeout = 0;
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "General error"
                previewSummary = qtTrId("fuoten-notify-general-error-summary");
                summary = previewSummary;
                previewBody = data.toString();
                body = previewBody;
                category = QStringLiteral("x-fuoten.error.general");
                break;
            case RequestError:
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QString));
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Request error"
                previewSummary = qtTrId("fuoten-notify-request-error-summary");
                summary = previewSummary;
                previewBody = data.toString();
                body = previewBody;
                expireTimeout = 0;
                category = QStringLiteral("x-fuoten.error.request");
                break;
            case ParsingError:
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QString));
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Parsing error"
                previewSummary = qtTrId("fuoten-notify-parsing-error-summary");
                summary = previewSummary;
                previewBody = data.toString();
                body = previewBody;
                expireTimeout = 0;
                category = QStringLiteral("x-fuoten.error.parsing");
                break;
            case InputError:
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QString));
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Input error"
                previewSummary = qtTrId("fuoten-notify-input-error-summary");
                summary = previewSummary;
                previewBody = data.toString();
                body = previewBody;
                expireTimeout = 0;
                category = QStringLiteral("x-fuoten.error.input");
                break;
            case OutputError:
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QString));
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Output error"
                previewSummary = qtTrId("fuoten-notify-output-error-summary");
                summary = previewSummary;
                previewBody = data.toString();
                body = previewBody;
                expireTimeout = 0;
                category = QStringLiteral("x-fuoten.error.output");
                break;
            case ServerError:
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QString));
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Server error"
                previewSummary = qtTrId("fuoten-notify-server-error-summary");
                summary = previewSummary;
                previewBody = data.toString();
                body = previewBody;
                expireTimeout = 0;
                category = QStringLiteral("x-fuoten.error.server");
                origin = m_config->getHost();
                break;
            case ApplicationError:
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QString));
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Application error"
                previewSummary = qtTrId("fuoten-notify-application-error-summary");
                summary = previewSummary;
                previewBody = data.toString();
                body = previewBody;
                expireTimeout = 0;
                category = QStringLiteral("x-fuoten.error.application");
                break;
            case StorageError:
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QString));
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Storage error"
                previewSummary = qtTrId("fuoten-notify-storage-error-summary");
                summary = previewSummary;
                previewBody = data.toString();
                body = previewBody;
                expireTimeout = 0;
                category = QStringLiteral("x-fuoten.error.storage");
                break;
            case AuthorizationError:
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QString));
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Authorization error"
                previewSummary = qtTrId("fuoten-notify-authorization-error-summary");
                summary = previewSummary;
                previewBody = data.toString();
                body = previewBody;
                expireTimeout = 0;
                category = QStringLiteral("x-fuoten.error.authorization");
                break;
            case SyncComplete:
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::Int));
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Synchronization complete"
                previewSummary = qtTrId("fuoten-notify-synccomplete-summary");
                summary = previewSummary;
                //: body notification message for both, notification popup and notification area
                //% "Synchronization succeeded in %n second(s)."
                previewBody = qtTrId("fuoten-notify-synccomplete-body", data.toInt());
                body = previewBody;
                expireTimeout = 5000;
                category = QStringLiteral("x-fuoten.sync.complete");
                break;
            case FoldersRequested:
            {
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QVariantList));
                const QVariantList folders = data.toList();
                const QStringList newFolders = folders.at(0).toStringList();
                const QStringList renamedFolders = folders.at(1).toStringList();
                const QStringList deletedFolders = folders.at(2).toStringList();
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Folders requested"
                previewSummary = qtTrId("fuoten-notify-folders-requested-prev-summary");
                summary = previewSummary;
                QStringList prevBodyParts;
                QLocale locale;
                if (!newFolders.empty()) {
                    //: part of the notification popup body text for requested folders, %n will be replaced by the numer of new folders
                    //% "%n new"
                    prevBodyParts.push_back(qtTrId("fuoten-notify-folders-requested-prev-body-new", newFolders.size()));
                    //: part of the notification area body text, %1 will be replaced by a list of affected folder names
                    //% "New folder(s): %1"
                    body.append(qtTrId("fuoten-notify-folders-requested-body-new", newFolders.size()).arg(locale.createSeparatedList(newFolders)));
                }
                if (!renamedFolders.empty()) {
                    //: part of the notification popup body text for requested folders, %n will be replaced by the numer of reanmed folders
                    //% "%n renamed"
                    prevBodyParts.push_back(qtTrId("fuoten-notify-folders-requested-prev-body-renamed", renamedFolders.size()));
                    if (!body.isEmpty()) {
                        body.append(QChar(QChar::LineFeed));
                    }
                    //: part of the notification area body text, %1 will be replaced by a list of affected folder names
                    //% "Renamed folder(s): %1"
                    body.append(qtTrId("fuoten-notify-folders-requested-body-renamed", renamedFolders.size()).arg(locale.createSeparatedList(renamedFolders)));
                }
                if (!deletedFolders.empty()) {
                    //: part of the notification popup body text for requested folders, %n will be replaced by the numer of deleted folders
                    //% "%n deleted"
                    prevBodyParts.push_back(qtTrId("fuoten-notify-folders-requested-prev-body-deleted", deletedFolders.size()));
                    if (!body.isEmpty()) {
                        body.append(QChar(QChar::LineFeed));
                    }
                    //: part of the notification area body text, %1 will be replaced by a list of affected folder names
                    //% "Deleted folder(s): %1"
                    body.append(qtTrId("fuoten-notify-folders-requested-body-deleted", deletedFolders.size()).arg(locale.createSeparatedList(deletedFolders)));
                }
                previewBody = locale.createSeparatedList(prevBodyParts);
                category = QStringLiteral("x-fuoten.folder.requested");
                break;
            }
            case FolderCreated:
            {
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QString));
                const QString createdFolderName = data.toString();
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Folder created"
                previewSummary = qtTrId("fuoten-notify-folder-created-summary");
                summary = previewSummary;
                previewBody = createdFolderName;
                //: notifaction area body text for a created folder, %1 will be replaced by the folder name
                //% "Folder “%1” successfully created."
                body = qtTrId("fuoten-notify-folder-created-body").arg(createdFolderName);
                expireTimeout = 5000;
                category = QStringLiteral("x-fuoten.folder.created");
                break;
            }
            case FolderDeleted:
            {
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QString));
                const QString deletedFolderName = data.toString();
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Folder deleted"
                previewSummary = qtTrId("fuoten-notify-folder-deleted-summary");
                summary = previewSummary;
                previewBody = deletedFolderName;
                //: notification area body text for a deleted folder, %1 will be replaced by the folder name
                //% "Folder “%1” successfully deleted."
                body = qtTrId("fuoten-notify-folder-deleted-body").arg(deletedFolderName);
                expireTimeout = 5000;
                category = QStringLiteral("x-fuoten.folder.deleted");
                break;
            }
            case FolderRenamed:
            {
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QStringList));
                const QStringList renamedFolderNames = data.toStringList();
                const QString oldName = renamedFolderNames.at(0);
                const QString newName = renamedFolderNames.at(1);
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Folder renamed"
                previewSummary = qtTrId("fuoten-notify-folder-renamed-summary");
                summary = previewSummary;
                //: notification popup body text for a renamed folder, %1 will be replaced by the old name, %2 by the new name
                //% "“%1” into “%2”"
                previewBody = qtTrId("fuoten-notify-folder-renamed-prev-body").arg(oldName, newName);
                //: notification area body text for a renamed folder, %1 will be replaced by the old name, %2 by the new name
                //% "Folder “%1” successfully renamed into “%2”.”"
                body = qtTrId("fuoten-notify-folder-renamed-body").arg(oldName, newName);
                expireTimeout = 5000;
                category = QStringLiteral("x-fuoten.folder.renamed");
                break;
            }
            case FolderMarkedRead:
            {
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QString));
                const QString folderMarkedReadName = data.toString();
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Folder marked as read"
                previewSummary = qtTrId("fuoten-notify-folder-markedread-summary");
                summary = previewSummary;
                previewBody = folderMarkedReadName;
                //: notification area body text for a folder marked as read, %1 will be replaced by the folder name
                //% "Folder “%1” successfully marked as read."
                body = qtTrId("fuoten-notify-folder-markedread-body").arg(folderMarkedReadName);
                expireTimeout = 5000;
                category = QStringLiteral("x-fuoten.folder.markedread");
                break;
            }
            case FeedsRequested:
            {
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QVariantList));
                const QVariantList reqFeeds = data.toList();
                const QStringList newFeeds = reqFeeds.at(0).toStringList();
                const QStringList updatedFeeds = reqFeeds.at(1).toStringList();
                const QStringList deletedFeeds = reqFeeds.at(2).toStringList();
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Feeds requested"
                previewSummary = qtTrId("fuoten-notify-feeds-requested-summary");
                summary = previewSummary;
                QStringList prevBodyParts;
                QLocale locale;
                if (!newFeeds.empty()) {
                    //: part of the notification popup body text for requested feeds, %n will be replaced by the numer of new feeds
                    //% "%n new"
                    prevBodyParts.push_back(qtTrId("fuoten-notify-feeds-requested-prev-body-new", newFeeds.size()));
                    //: part of the notification area body text for requested feeds, %1 will be replaced by a list of new feed titles
                    //% "New feed(s): %1"
                    body = qtTrId("fuoten-notify-feeds-requested-body-new", newFeeds.size()).arg(locale.createSeparatedList(newFeeds));
                }
                if (!updatedFeeds.empty()) {
                    //: part of the notification popup body text for requested feeds, %n will be replaced by the numer of updated feeds
                    //% "%n updated"
                    prevBodyParts.push_back(qtTrId("fuoten-notify-feeds-requested-prev-body-updated", updatedFeeds.size()));
                    if (!body.isEmpty()) {
                        body.append(QChar(QChar::LineFeed));
                    }
                    //: part of the notification area body text for requested feeds, %1 will be replaced by a list of updated feed titles
                    //% "Updated feed(s): %1"
                    body = qtTrId("fuoten-notify-feeds-requested-body-updated", updatedFeeds.size()).arg(locale.createSeparatedList(updatedFeeds));
                }
                if (!deletedFeeds.empty()) {
                    //: part of the notifitcation popup body text for requested feeds, %n will be replaced by the numer of deleted feeds
                    //% "%n deleted"
                    prevBodyParts.push_back(qtTrId("fuoten-notify-feeds-requested-prev-body-deleted", deletedFeeds.size()));
                    if (!body.isEmpty()) {
                        body.append(QChar(QChar::LineFeed));
                    }
                    //: part of the notification area body text for requested feeds, %1 will be replaced by a list of deleted feed titles
                    //% "Deleted feed(s): %1"
                    body = qtTrId("fuoten-notify-feeds-requested-body-deleted", deletedFeeds.size()).arg(locale.createSeparatedList(deletedFeeds));
                }
                previewBody = locale.createSeparatedList(prevBodyParts);
                category = QStringLiteral("x-fuoten.feed.requested");
                break;
            }
            case FeedCreated:
            {
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QString));
                const QString createdFeedName = data.toString();
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Feed added"
                previewSummary = qtTrId("fuoten-notify-feed-created-summary");
                summary = previewSummary;
                previewBody = createdFeedName;
                //: notification area body text for a newly created feed, %1 will be replaced by the feed title
                //% "Feed “%1” successfully added."
                body = qtTrId("fuoten-notify-feed-created-body").arg(createdFeedName);
                expireTimeout = 5000;
                category = QStringLiteral("x-fuoten.feed.created");
                break;
            }
            case FeedDeleted:
            {
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QString));
                const QString deletedFeedName = data.toString();
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Feed deleted"
                previewSummary = qtTrId("fuoten-notify-feed-deleted-summary");
                summary = previewSummary;
                previewBody = deletedFeedName;
                //: notification area body text for a deleted feed, %1 will be replaced by the feed title
                //% "Feed “%1” successfully deleted."
                body = qtTrId("fuoten-notify-feed-deleted-body").arg(deletedFeedName);
                expireTimeout = 5000;
                category = QStringLiteral("x-fuoten.feed.deleted");
                break;
            }
            case FeedMoved:
            {
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QStringList));
                const QStringList movedFeedList = data.toStringList();
                const QString movedFeedName = movedFeedList.at(0);
                const QString oldFeedFolder = movedFeedList.at(1);
                const QString newFeedFolder = movedFeedList.at(2);
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Feed moved"
                previewSummary = qtTrId("fuoten-notify-feed-moved-summary");
                summary = previewSummary;
                //: notification popup body text for a moved feed, %1 will be replaced by the old folder name, %2 by the new folder name
                //% "From folder “%1” into “%2”."
                previewBody = qtTrId("fuoten-notify-feed-moved-prev-body").arg(oldFeedFolder, newFeedFolder);
                //: notification area body text for a moved feed, %1 will be replaced by the feed title, %2 will be replaced by the old folder name, %3 by the new folder name
                //% "Feed “%1” successfully moved from folder “%2” into “%3”."
                body = qtTrId("fuoten-notify-feed-moved-body").arg(movedFeedName, oldFeedFolder, newFeedFolder);
                expireTimeout = 5000;
                category = QStringLiteral("x-fuoten.feed.moved");
                break;
            }
            case FeedRenamed:
            {
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QStringList));
                const QStringList rfl = data.toStringList();
                const QString oldFeedName = rfl.at(0);
                const QString newFeedName = rfl.at(1);
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Feed renamed"
                previewSummary = qtTrId("fuoten-notify-feed-renamed-summary");
                summary = previewSummary;
                //: notification popup body text, %1 will be replaced by the old feed name, %2 by the new feed name
                //% "Fom “%1” into “%2”."
                previewBody = qtTrId("fuoten-notify-feed-renamed-prev-body").arg(oldFeedName, newFeedName);
                //: notification area body text, %1 will be replaced by the old feed name, %2 by the new feed name
                //% "Feed “%1” successfully renamed into “%2”."
                body = qtTrId("fuoten-notify-feed-reanmed-body").arg(oldFeedName, newFeedName);
                expireTimeout = 5000;
                category = QStringLiteral("x-fuoten.feed.renamed");
                break;
            }
            case FeedMarkedRead:
            {
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QString));
                const QString markedFeedName = data.toString();
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Feed marked read"
                previewSummary = qtTrId("fuoten-notify-feed-markedread-summary");
                summary = previewSummary;
                previewBody = markedFeedName;
                //: notification area body text, %1 will be replaced by the feed title
                //% "Feed “%1” successfully marked as read."
                body = qtTrId("fuoten-notify-feed-markedread-body").arg(markedFeedName);
                expireTimeout = 5000;
                category = QStringLiteral("x-fuoten.feed.markedread");
                break;
            }
            case ItemsRequested:
            {
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::Int));
                const int unreadCount = data.toInt();
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Articles requested"
                previewSummary = qtTrId("fuoten-notify-items-requested-summary");
                summary = previewSummary;
                //: notification popup body text
                //% "%n new unread article(s)."
                previewBody = qtTrId("fuoten-notify-items-requested-prev-body", unreadCount);
                //: notification area body text
                //% "Successfully requested articles from the server. There are %n new unread article(s)."
                body = qtTrId("fuoten-notify-items-requested-body", unreadCount);
                category = QStringLiteral("x-fuoten.items.requested");
                break;
            }
            case AuthorizationSucceeded:
            {
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QVariantMap));
                const QVariantMap dataMap = data.toMap();
                const QString username = dataMap.value(QStringLiteral("username")).toString();
                const QString host = dataMap.value(QStringLiteral("host")).toString();
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Authorization successful"
                previewSummary = qtTrId("fuoten-notify-auth-success-summary");
                summary = previewSummary;
                //: notification popup body text after successful authorization, %1 will be replaced by the username, %2 by the remote Nextcloud host name
                //% "User %1 on %2"
                previewBody = qtTrId("fuoten-notify-auth-success-prev-body").arg(username, host);
                //: notification area body text after successful authorization, %1 will be replaced by the remote Nextcloud host name, %2 by the username
                //% "Fuoten has been successfully authorised to connect to your Nextcloud at %1 as user %2."
                body = qtTrId("fuoten-notify-auth-success-body").arg(host, username);
                expireTimeout = 5000;
                category = QStringLiteral("x-fuoten.authorization.success");
                break;
            }
            case AppPasswordRequested:
            {
                Q_ASSERT(data.isValid());
                Q_ASSERT(data.canConvert(QMetaType::QVariantMap));
                const QVariantMap dataMap = data.toMap();
                const QString username = dataMap.value(QStringLiteral("username")).toString();
                const QString host = dataMap.value(QStringLiteral("host")).toString();
                const QString useragent = dataMap.value(QStringLiteral("useragent")).toString();
                //: headline/summary for a notification, shown in the notification area and in the notification popup
                //% "Application password created"
                previewSummary = qtTrId("fuoten-notify-apppassword-created");
                summary = previewSummary;
                //: notification popup body text after successfully automatically creating an application password, %1 will be replaced by the username, %2 by the remote Nextcloud host name
                //% "User %1 on %2"
                previewBody = qtTrId("fuoten-notify-apppassword-created-prev-body").arg(username, host);
                //: notification area body text after successfully automatically converting to an application password, %1 will be replaced by the application’s login flow user agent (e.g. Fuoten (Xperia X)),  %2 will be replaced by the remote Nextcloud host name, %3 by the username
                //% "%1 has been successfully upgraded your login credentials to connect to your Nextcloud at %2 as user %3 with an application specific password."
                body = qtTrId("fuoten-notify-apppassword-created-body").arg(useragent, host, username);
                category = QStringLiteral("x-fuoten.authorization.upgraded");
                break;
            }
            }

            // TODO: use Notification::notificationsByCategory()
            const QList<QObject*> currentNotifications = Notification::notifications();

            if (!currentNotifications.empty()) {
                qDebug("Found %i previous notifications.", currentNotifications.size());
                for (QObject *o : currentNotifications) {
                    Notification *n = qobject_cast<Notification *>(o);
                    if (n && (n->category() == category)) {
                        qDebug("Trying to close notification with ID %i.", n->replacesId());
                        n->close();
                    }
                }
            }

            qDebug("Publishing new notification: %s", qUtf8Printable(previewSummary));

            // TODO: make actions static and check if it is empty
            QVariantList actions;
            actions.push_back(Notification::remoteAction(QStringLiteral("default"),
                                                         QStringLiteral(""),
                                                         QStringLiteral("org.harbour.fuoten"),
                                                         QStringLiteral("/"),
                                                         QStringLiteral("org.harbour.fuoten"),
                                                         QStringLiteral("activate")));
            actions.push_back(Notification::remoteAction(QStringLiteral("app"),
                                                         QStringLiteral(""),
                                                         QStringLiteral("org.harbour.fuoten"),
                                                         QStringLiteral("/"),
                                                         QStringLiteral("org.harbour.fuoten"),
                                                         QStringLiteral("activate")));

            Notification n;
            n.setAppIcon(appIcon());
            n.setAppName(appName());
            n.setBody(body);
            n.setCategory(category);
            n.setExpireTimeout(expireTimeout);
            n.setIcon(icon);
            n.setPreviewBody(previewBody);
            n.setPreviewSummary(previewSummary);
            n.setSummary(summary);
            n.setUrgency(urgency);
            n.setRemoteActions(actions);
            n.publish();
        }
    }
}

#include "moc_sfosnotificator.cpp"
