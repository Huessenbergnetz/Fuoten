/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "configuration.h"
#include <QRegularExpression>
#include <QImage>
#include <QDir>
#include <QStandardPaths>
#include <QSettings>
#include <QMetaEnum>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <cmath>
#include <algorithm>
#include <limits>

#define CONF_KEY_UPDATE_INTERVAL "behavior/updateInterval"
#define CONF_KEY_UPDATE_WLAN_ONLY "behavior/updateWlanOnly"
#define CONF_KEY_ARTICLE_FONT_SIZE "display/articleFontSize"
#define CONF_KEY_NOTIFICATIONS_ENABLED "behavior/notificationsEnabled"
#define CONF_KEY_NAVBACKAFTERMARK "behavior/navBackAfterMark"

/*!
 * \brief Constructs a new Configuration object.
 */
Configuration::Configuration(QObject *parent) :
    Fuoten::AbstractConfiguration(parent), m_settings(new QSettings(this))
{
    m_username = value(QStringLiteral("account/user")).toString();
    m_password = value(QStringLiteral("account/password")).toString();
    m_useSSL = value(QStringLiteral("account/usessl"), true).toBool();
    m_host = value(QStringLiteral("account/host")).toString();
    m_installPath = value(QStringLiteral("account/installpath")).toString();
    m_savedAppVersion = QVersionNumber::fromString(value(QStringLiteral("system/appVersion")).toString());
    m_serverPort = value(QStringLiteral("account/serverport"), 0).toInt();
    checkAccountValidity();
    m_ignoreSSLErrors = value(QStringLiteral("account/ignoresslerrors"), false).toBool();
    m_language = value(QStringLiteral("display/language")).toString();
    m_mainViewType = static_cast<Fuoten::FuotenEnums::Type>(value(QStringLiteral("display/mainViewType"), Fuoten::FuotenEnums::Folder).toInt());
    m_updateInterval = value(QStringLiteral(CONF_KEY_UPDATE_INTERVAL), 0).value<quint32>();
    m_wlanOnlyUpdate = value(QStringLiteral(CONF_KEY_UPDATE_WLAN_ONLY), true).toBool();
    m_articleFontSize = value(QStringLiteral(CONF_KEY_ARTICLE_FONT_SIZE), 0).toInt();
    m_notificationsEnabled = value(QStringLiteral(CONF_KEY_NOTIFICATIONS_ENABLED), true).toBool();
    m_navBackAfterMark = value(QStringLiteral(CONF_KEY_NAVBACKAFTERMARK), true).toBool();

    uint lsts = value(QStringLiteral("system/lastsync"), 0).toUInt();
    if (lsts > 0) {
        m_lastSync = QDateTime::fromTime_t(lsts).toUTC();
    }

    m_humanLastSync = getHumanLastSync();
}


/*!
 * \brief Deconstructs the Configuration object.
 */
Configuration::~Configuration() = default;


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
    return static_cast<Fuoten::FuotenEnums::ItemDeletionStrategy>(value(QStringLiteral("FeedItems_%1/deletionStrategy").arg(feedId), Fuoten::FuotenEnums::DeleteItemsByTime).toInt());
}



quint16 Configuration::getPerFeedDeletionValue(qint64 feedId) const
{
    return static_cast<quint16>(std::min(value(QStringLiteral("FeedItems_%1/deletionValue").arg(feedId), 14).toUInt(), static_cast<uint>(std::numeric_limits<quint16>::max())));
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

        float td = static_cast<float>(getLastSync().secsTo(QDateTime::currentDateTimeUtc()));

        if (td <= 10) {
            //: relative time for last synchronisation
            //% "just now"
            ret = qtTrId("fuoten-just-now");
        } else if (td < 60.0f) {
            //: relative time for last synchronisation
            //% "%n second(s) ago"
            ret = qtTrId("fuoten-seconds-ago", static_cast<int>(td));
        } else if (td < 7200.0f) {
            long rtd = std::lroundf(td/60.0f);
            //: relative time for last synchronisation
            //% "%n minute(s) ago"
            ret = qtTrId("fuoten-minutes-ago", rtd);
        } else if (td < 172800.0f) {
            long rtd = std::lroundf(td/3600.0f);
            //: relative time for last synchronisation
            //% "%n hour(s) ago"
            ret = qtTrId("fuoten-hours-ago", rtd);
        } else  {
            long rtd = std::lroundf(td/86400.0f);
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


bool Configuration::navBackAfterMark() const { return m_navBackAfterMark; }

void Configuration::setNavBackAfterMark(bool navBackAfterMark)
{
    if (m_navBackAfterMark != navBackAfterMark) {
        m_navBackAfterMark = navBackAfterMark;
        setValue(QStringLiteral(CONF_KEY_NAVBACKAFTERMARK), m_navBackAfterMark);
        qDebug("Changed navBackAfterMark to %s.", m_navBackAfterMark ? "true" : "false");
        emit navBackAfterMarkChanged(m_navBackAfterMark);
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

void Configuration::deleteAccount()
{
    qDebug("%s", "deleting account data");
    m_settings->beginGroup(QStringLiteral("account"));
    m_settings->remove(QStringLiteral(""));
    m_settings->endGroup();
    m_settings->remove(QStringLiteral("system/appVersion"));
    emit accountDeleted();
}

void Configuration::setValue(const QString &key, const QVariant &value)
{
    m_settings->setValue(key, value);
}

QVariant Configuration::value(const QString &key, const QVariant &defaultValue) const
{
    return m_settings->value(key, defaultValue);
}

void Configuration::remove(const QString &key)
{
    m_settings->remove(key);
}

#include "moc_configuration.cpp"
