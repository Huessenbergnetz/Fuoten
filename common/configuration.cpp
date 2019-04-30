/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2019 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
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
#include <math.h>
#include <QMetaEnum>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>

#define DEFAULT_AVATAR "image://theme/icon-l-people"
#define CONF_KEY_UPDATE_INTERVAL "behavior/updateInterval"
#define CONF_KEY_UPDATE_WLAN_ONLY "behavior/updateWlanOnly"
#define CONF_KEY_ARTICLE_FONT_SIZE "display/articleFontSize"
#define CONF_KEY_NOTIFICATIONS_ENABLED "behavior/notificationsEnabled"

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
    m_serverVersion = QVersionNumber::fromString(value(QStringLiteral("account/serverversion")).toString());
    m_savedAppVersion = QVersionNumber::fromString(value(QStringLiteral("system/appVersion")).toString());
    m_serverPort = value(QStringLiteral("account/serverport"), 0).toInt();
    m_improperlyConfiguredCron = value(QStringLiteral("warnings/improperlyConfiguredCron"), false).toBool();
    m_isAccountValid = false;
    checkAccountValidity();
    m_ignoreSSLErrors = value(QStringLiteral("account/ignoresslerrors"), false).toBool();
    m_avatar = value(QStringLiteral("account/avatar"), QStringLiteral(DEFAULT_AVATAR)).toUrl();
    m_language = value(QStringLiteral("display/language")).toString();
    m_mainViewType = (Fuoten::FuotenEnums::Type)value(QStringLiteral("display/mainViewType"), Fuoten::FuotenEnums::Folder).toInt();
    m_updateInterval = value(QStringLiteral(CONF_KEY_UPDATE_INTERVAL), 0).value<quint32>();
    m_wlanOnlyUpdate = value(QStringLiteral(CONF_KEY_UPDATE_WLAN_ONLY), true).toBool();
    m_articleFontSize = value(QStringLiteral(CONF_KEY_ARTICLE_FONT_SIZE), 0).toInt();
    m_notificationsEnabled = value(QStringLiteral(CONF_KEY_NOTIFICATIONS_ENABLED), true).toBool();

    uint lsts = value(QStringLiteral("system/lastsync"), 0).toUInt();
    if (lsts > 0) {
        m_lastSync = QDateTime::fromTime_t(lsts).toUTC();
    }

    m_humanLastSync = getHumanLastSync();
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
        qDebug("Changed username to %s.", qUtf8Printable(m_username));
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
        qDebug("Changed password to %s.", qUtf8Printable(m_password));
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
        qDebug("Changed useSSL to %s.", m_useSSL ? "true" : "false");
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
        qDebug("Changed host to %s.", qUtf8Printable(m_host));
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
        qDebug("Changed installPath to %s.", qUtf8Printable(m_installPath));
        setValue(QStringLiteral("account/installpath"), m_installPath);
        emit installPathChanged(getInstallPath());
    }
}




QString Configuration::displayName() const { return m_displayName; }

void Configuration::setDisplayName(const QString &nDisplayName)
{
    if (nDisplayName != m_displayName) {
        m_displayName = nDisplayName;
        qDebug("Changed displayName to %s.", qUtf8Printable(m_displayName));
        setValue(QStringLiteral("account/displayname"), m_displayName);
        emit displayNameChanged(displayName());
    }
}




bool Configuration::improperlyConfiguredCron() const { return m_improperlyConfiguredCron; }

void Configuration::setImproperlyConfiguredCron(bool nImproperlyConfiguredCron)
{
    if (nImproperlyConfiguredCron != m_improperlyConfiguredCron) {
        m_improperlyConfiguredCron = nImproperlyConfiguredCron;
        qDebug("Changed improperlyConfiguredCron to %s.", m_improperlyConfiguredCron ? "true" : "false");
        setValue(QStringLiteral("warnings/improperlyConfiguredCron"), m_improperlyConfiguredCron);
        emit improperlyConfiguredCronChanged(improperlyConfiguredCron());
    }
}


QVersionNumber Configuration::getServerVersion() const { return m_serverVersion; }

QString Configuration::serverVersion() const { return m_serverVersion.toString(); }

void Configuration::setServerVersion(const QString &nServerVersion)
{
    m_serverVersion = QVersionNumber::fromString(nServerVersion);
    setValue(QStringLiteral("account/serverversion"), m_serverVersion.toString());
    checkAccountValidity();
    emit serverVersionChanged(serverVersion());
}



bool Configuration::isAccountValid() const { return m_isAccountValid; }

void Configuration::setIsAccountValid(bool nIsAccountValid)
{
    if (nIsAccountValid != m_isAccountValid) {
        m_isAccountValid = nIsAccountValid;
        qDebug("Changed isAccountValid to %s.", m_isAccountValid ? "true" : "false");
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
        qDebug("Changed serverPort to %i.", m_serverPort);
        setValue(QStringLiteral("account/serverport"), m_serverPort);
        emit serverPortChanged(getServerPort());
    }
}



bool Configuration::getIgnoreSSLErrors() const { return m_ignoreSSLErrors; }

void Configuration::setIgnoreSSLErrors(bool ignoreSSLErrors)
{
    if (ignoreSSLErrors != m_ignoreSSLErrors) {
        m_ignoreSSLErrors = ignoreSSLErrors;
        qDebug("Changed ignoreSSLErrors to %s.", m_ignoreSSLErrors ? "true" : "false");
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
        qDebug("Changed language to %s.", qUtf8Printable(m_language));
        setValue(QStringLiteral("display/language"), m_language);
        emit languageChanged(language());
    }
}




Fuoten::FuotenEnums::Type Configuration::mainViewType() const { return m_mainViewType; }

void Configuration::setMainViewType(Fuoten::FuotenEnums::Type nMainViewType)
{
    if (nMainViewType != m_mainViewType) {
        m_mainViewType = nMainViewType;
        qDebug("Changed mainViewType to %s.", Fuoten::FuotenEnums::staticMetaObject.enumerator(Fuoten::FuotenEnums::staticMetaObject.indexOfEnumerator("Type")).valueToKey(m_mainViewType));
        setValue(QStringLiteral("display/mainViewType"), m_mainViewType);
        emit mainViewTypeChanged(mainViewType());
    }
}



QDateTime Configuration::getLastSync() const { return m_lastSync; }

void Configuration::setLastSync(const QDateTime &lastSync)
{
    if (lastSync != m_lastSync) {
        m_lastSync = lastSync;
        qDebug("Changed lastSync to %s.", qUtf8Printable(m_lastSync.toString(Qt::ISODate)));
        setValue(QStringLiteral("system/lastsync"), lastSync.toTime_t());
        Q_EMIT lastSyncChanged(getLastSync());
        setHumanLastSync(getHumanLastSync());
    }
}


Fuoten::FuotenEnums::ItemDeletionStrategy Configuration::getPerFeedDeletionStrategy(qint64 feedId) const
{
    return (Fuoten::FuotenEnums::ItemDeletionStrategy)value(QStringLiteral("FeedItems_%1/deletionStrategy").arg(feedId), Fuoten::FuotenEnums::DeleteItemsByTime).toInt();
}



quint16 Configuration::getPerFeedDeletionValue(qint64 feedId) const
{
    return value(QStringLiteral("FeedItems_%1/deletionValue").arg(feedId), 14).toUInt();
}



bool Configuration::checkForUpdate() const
{
    return m_savedAppVersion < QVersionNumber::fromString(QStringLiteral(VERSION_STRING));
}


bool Configuration::isFirstStart() const
{
    return m_savedAppVersion.isNull();
}


void Configuration::setCurrentVersion()
{
    setValue(QStringLiteral("system/appVersion"), QStringLiteral(VERSION_STRING));
}



QString Configuration::getHumanLastSync() const
{
    //: relative time for last synchronisation
    //% "never"
    QString ret = qtTrId("fuoten-sync-never");

    if (m_lastSync.toMSecsSinceEpoch() > 0) {

        qreal td = (qreal)getLastSync().secsTo(QDateTime::currentDateTimeUtc());

        if (td <= 10) {
            //: relative time for last synchronisation
            //% "just now"
            ret = qtTrId("fuoten-just-now");
        } else if (td < 60.0) {
            //: relative time for last synchronisation
            //% "%n second(s) ago"
            ret = qtTrId("fuoten-seconds-ago", td);
        } else if (td < 7200.0) {
            long int rtd = lround(td/60.0);
            //: relative time for last synchronisation
            //% "%n minute(s) ago"
            ret = qtTrId("fuoten-minutes-ago", rtd);
        } else if (td < 172800.0) {
            long int rtd = lround(td/3600.0);
            //: relative time for last synchronisation
            //% "%n hour(s) ago"
            ret = qtTrId("fuoten-hours-ago", rtd);
        } else  {
            long int rtd = lround(td/86400.0);
            //: relative time for last synchronisation
            //% "%n day(s) ago"
            ret = qtTrId("fuoten-days-ago", rtd);
        }
    }

    return ret;
}


quint32 Configuration::updateInterval() const { return m_updateInterval; }

void Configuration::setUpdateInterval(quint32 updateInterval)
{
    if (m_updateInterval != updateInterval) {
        m_updateInterval = updateInterval;
        setValue(QStringLiteral(CONF_KEY_UPDATE_INTERVAL), m_updateInterval);
        qDebug("Changed update interval to %u seconds.", m_updateInterval);
        emit updateIntervalChanged(m_updateInterval);
    }
}


QString Configuration::humanLastSync() const { return m_humanLastSync; }

void Configuration::setHumanLastSync(const QString &humanLastSync)
{
    if (m_humanLastSync != humanLastSync) {
        m_humanLastSync = humanLastSync;
        qDebug("Changed humanLastSync to \"%s\".", qUtf8Printable(m_humanLastSync));
        emit humanLastSyncChanged(m_humanLastSync);
    }
}


bool Configuration::wlanOnlyUpdate() const { return m_wlanOnlyUpdate; }

void Configuration::setWlanOnlyUpdate(bool wlanOnlyUpdate)
{
    if (m_wlanOnlyUpdate != wlanOnlyUpdate) {
        m_wlanOnlyUpdate = wlanOnlyUpdate;
        setValue(QStringLiteral(CONF_KEY_UPDATE_WLAN_ONLY), m_wlanOnlyUpdate);
        qDebug("Changed wlanOnlyUpdate to %s.", m_wlanOnlyUpdate ? "true" : "false");
        emit wlanOnlyUpdateChanged(m_wlanOnlyUpdate);
    }
}


int Configuration::articleFontSize() const { return m_articleFontSize; }

void Configuration::setArticleFontSize(int articleFontSize)
{
    if (m_articleFontSize != articleFontSize) {
        m_articleFontSize = articleFontSize;
        setValue(QStringLiteral(CONF_KEY_ARTICLE_FONT_SIZE), m_articleFontSize);
        qDebug("Changed articleFontSize to %i.", m_articleFontSize);
        emit articleFontSizeChanged(m_articleFontSize);
    }
}


bool Configuration::notificationsEnabled() const { return m_notificationsEnabled; }

void Configuration::setNotificationsEnabled(bool notificationsEnabled)
{
    if (m_notificationsEnabled != notificationsEnabled) {
        m_notificationsEnabled = notificationsEnabled;
        setValue(QStringLiteral(CONF_KEY_NOTIFICATIONS_ENABLED), m_notificationsEnabled);
        qDebug("Changed notificationsEnabled to %s.", m_notificationsEnabled ? "true" : "false");
        emit notificationsEnabledChanged(m_notificationsEnabled);
    }
}


bool Configuration::isUpdatePossible() const
{
    qDebug("%s", "Checking for update possibility.");
    if ((m_updateInterval > 0) && (m_lastSync.secsTo(QDateTime::currentDateTimeUtc()) > static_cast<qint64>(m_updateInterval))) {

        QNetworkConfigurationManager netConfManager;
        const QList<QNetworkConfiguration> netConfs = netConfManager.allConfigurations(QNetworkConfiguration::Active);

        if (!netConfs.empty()) {
            if (m_wlanOnlyUpdate) {
                for (const QNetworkConfiguration &conf : netConfs) {
                    switch(conf.bearerType()) {
                    case QNetworkConfiguration::BearerWLAN:
                    case QNetworkConfiguration::BearerEthernet:
                        return true;
                    default:
                        qDebug("%s", "Update will not be performed. No WLAN connection available.");
                        return false;
                    }
                }

            } else {
                for (const QNetworkConfiguration &conf : netConfs) {
                    switch(conf.bearerTypeFamily()) {
                    case QNetworkConfiguration::BearerWLAN:
                    case QNetworkConfiguration::BearerEthernet:
                    case QNetworkConfiguration::Bearer2G:
                    case QNetworkConfiguration::Bearer3G:
                    case QNetworkConfiguration::Bearer4G:
                        return true;
                    default:
                        qDebug("%s", "Update will not be performed. No network connection available.");
                        return false;
                    }
                }
            }
        }
    }

    qDebug("Update will not be performed. Last update is not %u seconds ago.", m_updateInterval);

    return false;
}


void Configuration::checkUpdate()
{
    setHumanLastSync(getHumanLastSync());
    if (!m_checkUpdateTimer) {
        m_checkUpdateTimer = new QTimer(this);
        m_checkUpdateTimer->setTimerType(Qt::VeryCoarseTimer);
        m_checkUpdateTimer->setSingleShot(true);
        connect(m_checkUpdateTimer, &QTimer::timeout, this, [this] () {
            if (isUpdatePossible()) {
                emit updatePossible();
            }
        });
        m_checkUpdateTimer->setInterval(5000);
    }
    m_checkUpdateTimer->start();
}

#include "moc_configuration.cpp"
