/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * common/configuration.cpp
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

#include "configuration.h"
#include <QRegularExpression>
#include <QImage>
#include <QDir>
#include <QStandardPaths>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

#define DEFAULT_AVATAR "image://theme/icon-l-people"

/*!
 * \brief Constructs a new Configuration object.
 */
Configuration::Configuration(QObject *parent) :
    Fuoten::AbstractConfiguration(parent)
{
    m_username = value(QStringLiteral("account/user")).toString();
    m_password = value(QStringLiteral("account/password")).toString();
    m_useSSL = value(QStringLiteral("account/usessl"), true).toBool();
    m_host = value(QStringLiteral("account/host")).toString();
    m_installPath = value(QStringLiteral("account/installpath")).toString();
    m_displayName = value(QStringLiteral("account/displayname")).toString();
#if QT_VERSION < QT_VERSION_CHECK(5, 6 ,0)
    m_serverVersion = Fuoten::VersionNumber(value(QStringLiteral("account/serverversion")).toString());
    m_savedAppVersion = Fuoten::VersionNumber(value(QStringLiteral("system/appVersion")).toString());
#else
    m_serverVersion = QVersionNumber::fromString(value(QStringLiteral("account/serverversion")).toString());
    m_savedAppVersion = QVersionNumber::fromString(value(QStringLiteral("system/appVersion")).toString());
#endif
    m_serverPort = value(QStringLiteral("account/serverport"), 0).toInt();
    m_improperlyConfiguredCron = value(QStringLiteral("warnings/improperlyConfiguredCron"), false).toBool();
    m_isAccountValid = false;
    checkAccountValidity();
    m_ignoreSSLErrors = value(QStringLiteral("account/ignoresslerrors"), false).toBool();
    m_avatar = value(QStringLiteral("account/avatar"), QStringLiteral(DEFAULT_AVATAR)).toUrl();
    m_language = value(QStringLiteral("display/language")).toString();
    m_mainViewType = (Fuoten::FuotenEnums::Type)value(QStringLiteral("display/mainViewType"), Fuoten::FuotenEnums::Feed).toInt();
    m_mainViewSorting = (Fuoten::FuotenEnums::SortingRole)value(QStringLiteral("display/mainViewSorting"), Fuoten::FuotenEnums::Name).toInt();
    m_mainViewHideRead = value(QStringLiteral("display/mainViewHideRead"), false).toBool();
    m_mainViewSortOrder = (Qt::SortOrder)value(QStringLiteral("display/mainViewSortOrder"), Qt::AscendingOrder).toInt();
    m_lastSync = QDateTime::fromTime_t(value(QStringLiteral("system/lastsync"), 0).toUInt());
}


/*!
 * \brief Deconstructs the Configuration object.
 */
Configuration::~Configuration()
{
}


QString Configuration::getUsername() const { return m_username; }

void Configuration::setUsername(const QString &username)
{
    if (username != m_username) {
        m_username = username;
#ifdef QT_DEBUG
        qDebug() << "Changed username to" << m_username;
#endif
        setValue(QStringLiteral("account/user"), m_username);
        checkAccountValidity();
        emit usernameChanged(getUsername());
    }
}




QString Configuration::getPassword() const { return m_password; }

void Configuration::setPassword(const QString &password)
{
    if (password != m_password) {
        m_password = password;
#ifdef QT_DEBUG
        qDebug() << "Changed password to" << m_password;
#endif
        setValue(QStringLiteral("account/password"), m_password);
        checkAccountValidity();
        emit passwordChanged(getPassword());
    }
}




bool Configuration::getUseSSL() const { return m_useSSL; }

void Configuration::setUseSSL(bool useSSL)
{
    if (useSSL != m_useSSL) {
        m_useSSL = useSSL;
#ifdef QT_DEBUG
        qDebug() << "Changed useSSL to" << m_useSSL;
#endif
        setValue(QStringLiteral("account/usessl"), m_useSSL);
        emit useSSLChanged(getUseSSL());
    }
}




QString Configuration::getHost() const { return m_host; }

void Configuration::setHost(const QString &host)
{
    QString _h = host.toLower();
    _h.remove(QRegularExpression(QStringLiteral("https?://")));
    _h.remove(QRegularExpression(QStringLiteral("/$")));
    if (_h != m_host) {
        m_host = _h;
#ifdef QT_DEBUG
        qDebug() << "Changed host to" << m_host;
#endif
        setValue(QStringLiteral("account/host"), m_host);
        checkAccountValidity();
        emit hostChanged(getHost());
    }
}




QString Configuration::getInstallPath() const { return m_installPath; }

void Configuration::setInstallPath(const QString &installPath)
{
    QString _ip = installPath;
    if (!_ip.startsWith(QChar('/'))) {
        _ip.prepend(QChar('/'));
    }
    if (_ip.endsWith(QChar('/'))) {
        _ip.chop(1);
    }
    if (_ip != m_installPath) {
        m_installPath = _ip;
#ifdef QT_DEBUG
        qDebug() << "Changed installPath to" << m_installPath;
#endif
        setValue(QStringLiteral("account/installpath"), m_installPath);
        emit installPathChanged(getInstallPath());
    }
}




QString Configuration::displayName() const { return m_displayName; }

void Configuration::setDisplayName(const QString &nDisplayName)
{
    if (nDisplayName != m_displayName) {
        m_displayName = nDisplayName;
#ifdef QT_DEBUG
        qDebug() << "Changed displayName to" << m_displayName;
#endif
        setValue(QStringLiteral("account/displayname"), m_displayName);
        emit displayNameChanged(displayName());
    }
}




bool Configuration::improperlyConfiguredCron() const { return m_improperlyConfiguredCron; }

void Configuration::setImproperlyConfiguredCron(bool nImproperlyConfiguredCron)
{
    if (nImproperlyConfiguredCron != m_improperlyConfiguredCron) {
        m_improperlyConfiguredCron = nImproperlyConfiguredCron;
#ifdef QT_DEBUG
        qDebug() << "Changed improperlyConfiguredCron to" << m_improperlyConfiguredCron;
#endif
        setValue(QStringLiteral("warnings/improperlyConfiguredCron"), m_improperlyConfiguredCron);
        emit improperlyConfiguredCronChanged(improperlyConfiguredCron());
    }
}


QVersionNumber Configuration::getServerVersion() const { return m_serverVersion; }

QString Configuration::serverVersion() const { return m_serverVersion.toString(); }

void Configuration::setServerVersion(const QString &nServerVersion)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    m_serverVersion = QVersionNumber::fromString(nServerVersion);
#else
    m_serverVersion.loadFromString(nServerVersion);
#endif
    setValue(QStringLiteral("account/serverversion"), m_serverVersion.toString());
    checkAccountValidity();
    emit serverVersionChanged(serverVersion());
}



bool Configuration::isAccountValid() const { return m_isAccountValid; }

void Configuration::setIsAccountValid(bool nIsAccountValid)
{
    if (nIsAccountValid != m_isAccountValid) {
        m_isAccountValid = nIsAccountValid;
#ifdef QT_DEBUG
        qDebug() << "Changed isAccountValid to" << m_isAccountValid;
#endif
        emit isAccountValidChanged(isAccountValid());
    }
}


QString Configuration::getUserAgent() const
{
    return QStringLiteral("Fuoten %1").arg(QStringLiteral(VERSION_STRING));
}


int Configuration::getServerPort() const { return m_serverPort; }

void Configuration::setServerPort(int serverPort)
{
    if (serverPort != m_serverPort) {
        m_serverPort = serverPort;
#ifdef QT_DEBUG
        qDebug() << "Changed serverPort to" << m_serverPort;
#endif
        setValue(QStringLiteral("account/serverport"), m_serverPort);
        emit serverPortChanged(getServerPort());
    }
}



bool Configuration::getIgnoreSSLErrors() const { return m_ignoreSSLErrors; }

void Configuration::setIgnoreSSLErrors(bool ignoreSSLErrors)
{
    if (ignoreSSLErrors != m_ignoreSSLErrors) {
        m_ignoreSSLErrors = ignoreSSLErrors;
#ifdef QT_DEBUG
        qDebug() << "Changed ignoreSSLErrors to" << m_ignoreSSLErrors;
#endif
        setValue(QStringLiteral("account/ignoresslerrors"), m_ignoreSSLErrors);
        emit ignoreSSLErrorsChanged(getIgnoreSSLErrors());
    }
}


QUrl Configuration::avatar() const { return m_avatar; }


void Configuration::setAvatar(const QString &data, const QString &mime)
{
    QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

    if (!dataDir.exists()) {
        if (!dataDir.mkpath(dataDir.absolutePath()))
        qCritical("Failed to create data directory.");
        return;
    } else {
        QStringList oldAvatars = dataDir.entryList(QStringList({QStringLiteral("avatar.*")}), QDir::Files);
        if (!oldAvatars.isEmpty()) {
            for (const QString &oa : oldAvatars) {
                dataDir.remove(oa);
            }
        }
    }

    if (data.isEmpty() || mime.isEmpty()) {

        qWarning("Data or mime is empty, no avatar data found.");
        m_avatar.setUrl(QStringLiteral(DEFAULT_AVATAR));
        remove(QStringLiteral("account/avatar"));
        emit avatarChanged(m_avatar);

    } else {

        QString type;
        type.reserve(3);

        if (mime.contains(QStringLiteral("png"), Qt::CaseInsensitive)) {
            type = QStringLiteral("PNG");
        } else if (mime.contains(QRegularExpression(QStringLiteral("jpe?g"), QRegularExpression::CaseInsensitiveOption))) {
            type = QStringLiteral("JPG");
        } else if (mime.contains(QStringLiteral("bmp"), Qt::CaseInsensitive)) {
            type = QStringLiteral("BMP");
        }

        if (type.isEmpty()) {
            qCritical("Unsupported image type %s", qPrintable(mime));

            m_avatar.setUrl(QStringLiteral(DEFAULT_AVATAR));
            remove(QStringLiteral("account/avatar"));
            emit avatarChanged(m_avatar);
            return;
        }

        QImage avatar;
        QByteArray ba;
        ba.append(data);

        if (avatar.loadFromData(QByteArray::fromBase64(ba), type.toUtf8().constData())) {

            const QString avatarPath = dataDir.absoluteFilePath(QStringLiteral("avatar.").append(type.toLower()));
            if (avatar.save(avatarPath), type.toUtf8().constData()) {
                m_avatar.setUrl(avatarPath);
                setValue(QStringLiteral("account/avatar"), avatarPath);
            } else {
                m_avatar.setUrl(QStringLiteral(DEFAULT_AVATAR));
                remove(QStringLiteral("account/avatar"));
            }
            emit avatarChanged(m_avatar);

        } else {
            qCritical("Can not load avatar image.");

            m_avatar.setUrl(QStringLiteral(DEFAULT_AVATAR));
            remove(QStringLiteral("account/avatar"));
            emit avatarChanged(m_avatar);
            return;
        }
    }

}



QString Configuration::language() const { return m_language; }

void Configuration::setLanguage(const QString &nLanguage)
{
    if (nLanguage != m_language) {
        m_language = nLanguage;
#ifdef QT_DEBUG
        qDebug() << "Changed language to" << m_language;
#endif
        setValue(QStringLiteral("display/language"), m_language);
        emit languageChanged(language());
    }
}




Fuoten::FuotenEnums::Type Configuration::mainViewType() const { return m_mainViewType; }

void Configuration::setMainViewType(Fuoten::FuotenEnums::Type nMainViewType)
{
    if (nMainViewType != m_mainViewType) {
        m_mainViewType = nMainViewType;
#ifdef QT_DEBUG
        qDebug() << "Changed mainViewType to" << m_mainViewType;
#endif
        setValue(QStringLiteral("display/mainViewType"), m_mainViewType);
        emit mainViewTypeChanged(mainViewType());
    }
}


Fuoten::FuotenEnums::SortingRole Configuration::mainViewSorting() const { return m_mainViewSorting; }

void Configuration::setMainViewSorting(Fuoten::FuotenEnums::SortingRole nMainViewSorting)
{
    if (nMainViewSorting != m_mainViewSorting) {
        m_mainViewSorting = nMainViewSorting;
#ifdef QT_DEBUG
        qDebug() << "Changed mainViewSorting to" << m_mainViewSorting;
#endif
        setValue(QStringLiteral("display/mainViewSorting"), m_mainViewSorting);
        emit mainViewSortingChanged(mainViewSorting());
    }
}



bool Configuration::mainViewHideRead() const { return m_mainViewHideRead; }

void Configuration::setMainViewHideRead(bool nMainViewHideRead)
{
    if (nMainViewHideRead != m_mainViewHideRead) {
        m_mainViewHideRead = nMainViewHideRead;
#ifdef QT_DEBUG
        qDebug() << "Changed mainViewHideRead to" << m_mainViewHideRead;
#endif
        setValue(QStringLiteral("display/mainViewHideRead"), m_mainViewHideRead);
        emit mainViewHideReadChanged(mainViewHideRead());
    }
}



Qt::SortOrder Configuration::mainViewSortOrder() const { return m_mainViewSortOrder; }

void Configuration::setMainViewSortOrder(Qt::SortOrder nMainViewSortOrder)
{
    if (nMainViewSortOrder != m_mainViewSortOrder) {
        m_mainViewSortOrder = nMainViewSortOrder;
#ifdef QT_DEBUG
        qDebug() << "Changed mainViewSortOrder to" << m_mainViewSortOrder;
#endif
        setValue(QStringLiteral("display/mainViewSortOrder"), m_mainViewSortOrder);
        emit mainViewSortOrderChanged(mainViewSortOrder());
    }
}


QDateTime Configuration::getLastSync() const { return m_lastSync; }

void Configuration::setLastSync(const QDateTime &lastSync)
{
    if (lastSync != m_lastSync) {
        m_lastSync = lastSync;
#ifdef QT_DEBUG
        qDebug() << "Changed lastSync to" << m_lastSync;
#endif
        setValue(QStringLiteral("system/lastsync"), lastSync.toTime_t());
        Q_EMIT lastSyncChanged(getLastSync());
    }
}




bool Configuration::checkForUpdate() const
{
#if QT_VERSION < QT_VERSION_CHECK(5, 6 ,0)
    return m_savedAppVersion < QVersionNumber(QStringLiteral(VERSION_STRING));
#else
    return m_savedAppVersion < QVersionNumber::fromString(QStringLiteral(VERSION_STRING));
#endif
}


bool Configuration::isFirstStart() const
{
    return m_savedAppVersion.isNull();
}


void Configuration::setCurrentVersion()
{
    setValue(QStringLiteral("system/appVersion"), QStringLiteral(VERSION_STRING));
}
