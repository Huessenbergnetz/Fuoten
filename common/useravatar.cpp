/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "useravatar.h"
#include <Fuoten/API/GetUser>
#include <Fuoten/Error>
#include <QRegularExpression>
#include <QImage>
#include <QDir>
#include <QStandardPaths>
#include <QFileInfoList>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QImage>
#include <QFile>

UserAvatar::UserAvatar(QObject *parent) : QObject(parent)
{

}

UserAvatar::~UserAvatar() = default;

void UserAvatar::check()
{
    if (inOperation()) {
        qWarning("%s", "Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Checking for user avatar image.");

    setInOperation(true);

    QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));

    if (!dataDir.exists()) {
        if (!dataDir.mkpath(dataDir.absolutePath())) {
            qCritical("%s", "Failed to create data directory.");
            setDefaultAvatarUrl();
            setInOperation(false);
            return;
        }
    }

    bool checkForNew = false;

    const QFileInfoList oldAvatars = dataDir.entryInfoList(QStringList({QStringLiteral("avatar.*")}), QDir::Files);

    if (oldAvatars.size() != 1) {
        qWarning("Found %i avatar files but expected exactly one!", oldAvatars.size());
        checkForNew = true;
    } else {
        const QFileInfo fi = oldAvatars.front();

        if (fi.lastModified().secsTo(QDateTime::currentDateTime()) > m_cachePeriod || fi.size() < 128) {
            qWarning("%s", "Avatar file not found or empty. Requesting new.");
            checkForNew = true;
        }
    }

    if (!checkForNew) {
        qDebug("%s", "Found existing avatar file.");
        QUrl url;
        url.setScheme(QStringLiteral("file"));
        url.setPath(oldAvatars.front().canonicalFilePath());
        setAvatarUrl(url);
        setInOperation(false);
        return;
    }

    auto getUser = new Fuoten::GetUser(this);
    connect(getUser, &Fuoten::GetUser::succeeded, this, &UserAvatar::gotUser);
    connect(getUser, &Fuoten::GetUser::succeeded, getUser, &Fuoten::GetUser::deleteLater);
    connect(getUser, &Fuoten::GetUser::failed, this, &UserAvatar::gotUserFailed);
    connect(getUser, &Fuoten::GetUser::failed, getUser, &Fuoten::GetUser::deleteLater);
    getUser->execute();
}

void UserAvatar::gotUser(const QJsonDocument &json)
{
    const QJsonObject avatarObject = json.object().value(QStringLiteral("avatar")).toObject();
    if (avatarObject.empty()) {
        qWarning("%s", "Returned data does not contain an avatar image.");
        setOldOrDefaultAvatarUrl();
        setInOperation(false);
        return;
    }

    const QString data = avatarObject.value(QStringLiteral("data")).toString();
    if (data.isEmpty()) {
        qWarning("%s", "Returned avatar image data is empty.");
        setOldOrDefaultAvatarUrl();
        setInOperation(false);
        return;
    }

    const QString mime = avatarObject.value(QStringLiteral("mime")).toString();
    if (mime.isEmpty()) {
        qWarning("%s", "Returned avatar image mime type is empty.");
        setOldOrDefaultAvatarUrl();
        setInOperation(false);
        return;
    }

    QByteArray type;
    type.reserve(3);

    if (mime.contains(QLatin1String("png"), Qt::CaseInsensitive)) {
        type = QByteArrayLiteral("PNG");
    } else if (mime.contains(QRegularExpression(QStringLiteral("jpe?g"), QRegularExpression::CaseInsensitiveOption))) {
        type = QByteArrayLiteral("JPG");
    } else if (mime.contains(QLatin1String("bmp"), Qt::CaseInsensitive)) {
        type = QByteArrayLiteral("BMP");
    }

    if (type.isEmpty()) {
        qWarning("Unsupported avatar image mime type %s", qUtf8Printable(mime));
        setOldOrDefaultAvatarUrl();
        setInOperation(false);
        return;
    }

    QImage image;

    if (image.loadFromData(QByteArray::fromBase64(data.toLatin1()), type.constData())) {
        const QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));

        const QStringList oldAvatars = dataDir.entryList(QStringList({QStringLiteral("avatar.*")}), QDir::Files);
        for (const QString &oldAvatar : oldAvatars) {
            qDebug("Removing old avatar image file at \"%s\".", qUtf8Printable(oldAvatar));
            if (Q_UNLIKELY(!QFile::remove(oldAvatar))) {
                qCritical("Failed to remove old avatar image file at \"%s\".", qUtf8Printable(oldAvatar));
                setDefaultAvatarUrl();
                setInOperation(false);
                return;
            }
        }

        const QString avatarPath = dataDir.absoluteFilePath(QStringLiteral("avatar.") + type.toLower());

        if (!image.save(avatarPath, type.constData())) {
            qCritical("Failed to save avatar image to \"%s\".", qUtf8Printable(avatarPath));
            setOldOrDefaultAvatarUrl();
            setInOperation(false);
            return;
        }

        QUrl url;
        url.setScheme(QStringLiteral("file"));
        url.setPath(avatarPath);
        setAvatarUrl(url);
        setInOperation(false);

    } else {
        qCritical("%s", "Failed to load avatar image data.");
        setOldOrDefaultAvatarUrl();
        setInOperation(false);
    }
}

void UserAvatar::gotUserFailed()
{
    setOldOrDefaultAvatarUrl();
}

QUrl UserAvatar::defaultAvatarUrl() const
{
    return QUrl();
}

void UserAvatar::setDefaultAvatarUrl()
{
    qDebug("%s", "Setting default avatar.");
    setAvatarUrl(defaultAvatarUrl());
}

void UserAvatar::setOldOrDefaultAvatarUrl()
{
    const QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    const QFileInfoList oldAvatars = dataDir.entryInfoList(QStringList({QStringLiteral("avatar.*")}), QDir::Files);

    for (const QFileInfo &fi : oldAvatars) {
        if (fi.size() > 128) {
            qDebug("%s", "Setting old avatar.");
            QUrl url;
            url.setScheme(QStringLiteral("file"));
            url.setPath(fi.canonicalFilePath());
            setAvatarUrl(url);
            return;
        }
    }

    setDefaultAvatarUrl();
}

QUrl UserAvatar::avatarUrl() const
{
    return m_avatarUrl;
}

void UserAvatar::setAvatarUrl(const QUrl &avatarUrl)
{
    if (m_avatarUrl != avatarUrl) {
        qDebug("Changing avatarUrl from \"%s\" to \"%s\"", qUtf8Printable(m_avatarUrl.toString()), qUtf8Printable(avatarUrl.toString()));
        m_avatarUrl = avatarUrl;
        emit avatarUrlChanged(m_avatarUrl);
    }
}

bool UserAvatar::inOperation() const
{
    return m_inOperation;
}

void UserAvatar::setInOperation(bool inOperation)
{
    if (m_inOperation != inOperation) {
        qDebug("Changing inOperation to %s.", inOperation ? "true" : "false");
        m_inOperation = inOperation;
        emit inOperationChanged(m_inOperation);
    }
}

int UserAvatar::cachePeriod() const
{
    return m_cachePeriod;
}

void UserAvatar::setCachePeriod(int cachePeriod)
{
    if (m_cachePeriod != cachePeriod) {
        qDebug("Changing cachePeriod from %i to %i seconds.", m_cachePeriod, cachePeriod);
        m_cachePeriod = cachePeriod;
        emit cachePeriodChanged(m_cachePeriod);
    }
}

#include "moc_useravatar.cpp"
