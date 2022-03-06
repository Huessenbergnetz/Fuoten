/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QVariant>
#include <QUrl>
#include <QDateTime>
#include <QTimer>
#include <Fuoten/Helpers/AbstractConfiguration>
#include <Fuoten/FuotenEnums>
#include <QVersionNumber>
#include "enums.h"

class QSettings;

/*!
 * \brief Provides the configuration interface for Fuoten.
 */
class Configuration : public Fuoten::AbstractConfiguration
{
    Q_OBJECT
    /*!
     * \brief The username used to authenticate.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>getUsername() const</TD></TR><TR><TD>void</TD><TD>setUsername(const QString &username)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>usernameChanged(const QString &username)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString username READ getUsername WRITE setUsername NOTIFY usernameChanged)
    /*!
     * \brief The user's password used for authentication.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>getPassword() const</TD></TR><TR><TD>void</TD><TD>setPassword(const QString &password)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>passwordChanged(const QString &password)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString password READ getPassword WRITE setPassword NOTIFY passwordChanged)
    /*!
     * \brief Set this to false to not use SSL/TLS.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>getUseSSL() const</TD></TR><TR><TD>void</TD><TD>setUseSSL(bool useSSL)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>useSSLChanged(bool useSSL)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool useSSL READ getUseSSL WRITE setUseSSL NOTIFY useSSLChanged)
    /*!
     * \brief The host of the cloud installation.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>getHost() const</TD></TR><TR><TD>void</TD><TD>setHost(const QString &host)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>hostChanged(const QString &host)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString host READ getHost WRITE setHost NOTIFY hostChanged)
    /*!
     * \brief The installation path of the cloud.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>getInstallPath() const</TD></TR><TR><TD>void</TD><TD>setInstallPath(const QString &installPath)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>installPathChanged(const QString &installPath)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString installPath READ getInstallPath WRITE setInstallPath NOTIFY installPathChanged)
    /*!
     * \brief Returns true if the account data seems to be valid.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>isAccountValid() const</TD></TR></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>isAccountValidChanged(bool isAccountValid)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool isAccountValid READ isAccountValid NOTIFY isAccountValidChanged)
    /*!
     * \brief Server port.
     *
     * \par Access functions:
     * <TABLE><TR><TD>int</TD><TD>getServerPort() const</TD></TR><TR><TD>void</TD><TD>setServerPort(int serverPort)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>serverPortChanged(int serverPort)</TD></TR></TABLE>
     */
    Q_PROPERTY(int serverPort READ getServerPort WRITE setServerPort NOTIFY serverPortChanged)
    /*!
     * \brief Set to true to ignore SSL erros. Be careful!
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>getIgnoreSSLErrors() const</TD></TR><TR><TD>void</TD><TD>setIgnoreSSLErrors(bool ignoreSSLErrors)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>ignoreSSLErrorsChanged(bool ignoreSSLErrors)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool ignoreSSLErrors READ getIgnoreSSLErrors WRITE setIgnoreSSLErrors NOTIFY ignoreSSLErrorsChanged)
    /*!
     * \brief Stores the user defined language.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>language() const</TD></TR><TR><TD>void</TD><TD>setLanguage(const QString &nLanguage)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>languageChanged(const QString &language)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    /*!
     * \brief Sets the type of the application main view page.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Fuoten::FuotenEnums::Type</TD><TD>mainViewType() const</TD></TR><TR><TD>void</TD><TD>setMainViewType(Fuoten::FuotenEnums::Type nMainViewType)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>mainViewTypeChanged(Fuoten::FuotenEnums::Type mainViewType)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::FuotenEnums::Type mainViewType READ mainViewType WRITE setMainViewType NOTIFY mainViewTypeChanged)
    /*!
     * \brief Stores the date and time of the last full sync.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QDateTime</TD><TD>getLastSync() const</TD></TR><TR><TD>void</TD><TD>setLastSync(const QDateTime &lastSync)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>lastSyncChanged(const QDateTime &lastSync)</TD></TR></TABLE>
     */
    Q_PROPERTY(QDateTime lastSync READ getLastSync WRITE setLastSync NOTIFY lastSyncChanged)
    /*!
     * \brief This property holds the automatic update interval in seconds.
     *
     * Set it to \c 0 to disable automatic updates.
     *
     * \par Access functions:
     * \li quint32 updateInterval() const
     * \li void setUpdateInerval(quint32 interval)
     *
     * \par Notifier signal:
     * \li void updateIntervalChanged(quint32 interval)
     */
    Q_PROPERTY(quint32 updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)
    /*!
     * \brief This property holds a human readable relative last synchronization time.
     *
     * The string will be something like "10 minutes ago".
     *
     * \par Access functions:
     * \li QString humanLastSync() const
     *
     * \par Notifier signal:
     * \li void humanLastSyncChanged(const QString &humanLastSync)
     */
    Q_PROPERTY(QString humanLastSync READ humanLastSync NOTIFY humanLastSyncChanged)
    /*!
     * \brief This property holds \c true if automatic updates should only be done on WLAN connections.
     *
     * \par Access functions:
     * \li bool wlanOnlyUpdate() const
     * \li void setWlanOnlyUpdate(bool wlanOnlyUpdate)
     *
     * \par Notifier signal:
     * \li void wlanOnlyUpdateChanged(bool wlanOnlyUpdate)
     */
    Q_PROPERTY(bool wlanOnlyUpdate READ wlanOnlyUpdate WRITE setWlanOnlyUpdate NOTIFY wlanOnlyUpdateChanged)
    /*!
     * \brief Holds the font size in px used for displaying article contentn internally.
     *
     * \par Access functions:
     * \li int articleFontSize() const
     * \li void setArticleFontSize(int articleFontSize)
     *
     * \par Notifier signal:
     * \li void articleFontSizeChanged(int articleFontSize)
     */
    Q_PROPERTY(int articleFontSize READ articleFontSize WRITE setArticleFontSize NOTIFY articleFontSizeChanged)
    /*!
     * \brief This property holds \c true if notifications are enabled.
     *
     * \par Access functions:
     * \li bool notificationsEnabled() const
     * \li void setNotificationsEnabled(bool notificationsEnabled)
     *
     * \par Notifier signal:
     * \li void notificationsEnabledChanged(bool notificationsEnabled)
     */
    Q_PROPERTY(bool notificationsEnabled READ notificationsEnabled WRITE setNotificationsEnabled NOTIFY notificationsEnabledChanged)
    /*!
     * \brief This property holds \c true if should navigated back after marking read.
     *
     * If \c true, after marking an entire feed or folder as read, go back to the previous page.
     *
     * \par Access functions:
     * \li bool navBackAfterMark() const
     * \li void setNavBackAfterMark(bool navBackAfterMark)
     *
     * \par Notifier signal:
     * \li void goBackOnReadAllChanged(bool navBackAfterMark)
     */
    Q_PROPERTY(bool navBackAfterMark READ navBackAfterMark WRITE setNavBackAfterMark NOTIFY navBackAfterMarkChanged)
    /*!
     * \brief This property holds wether JavaScript should be supported by default in the internal web view.
     *
     * The default values is FuotenAppEnums::JsEnabled.
     *
     * \par Access functions:
     * \li FuotenAppEnums::JsSupport jsSupport() const
     * \li setJsSupport(FuotenAppEnums::JsSupport jsSupport)
     *
     * \par Notifier signal:
     * \li void jsSupportChanged(FuotenAppEnums::JsSupport jsSupport)
     */
    Q_PROPERTY(FuotenAppEnums::JsSupport jsSupport READ jsSupport WRITE setJsSupport NOTIFY jsSupportChanged)
    /*!
     * \brief This property holds how to handle cookies by default.
     *
     * The default values is FuotenAppEnums::CookiesAcceptAll.
     *
     * \par Access functions:
     * \li FuotenAppEnums::CookieBehavior cookieBehavior() const
     * \li void setCookieBehavior(FuotenAppEnums::CookieBehavior cookieBehavior)
     *
     * \par Notifier signal:
     * \li void cookieBehaviorChanged(FuotenAppEnums::CookieBehavior cookieBehavior)
     */
    Q_PROPERTY(FuotenAppEnums::CookieBehavior cookieBehavior READ cookieBehavior WRITE setCookieBehavior NOTIFY cookieBehaviorChanged)
    Q_ENUM(Fuoten::FuotenEnums::Type)
public:
    explicit Configuration(const QString &settingsPath = QString(), QObject *parent = nullptr);
    ~Configuration() override;

    QString getUsername() const override;
    QString getPassword() const override;
    bool getUseSSL() const override;
    QString getHost() const override;
    QString getInstallPath() const override;
    bool isAccountValid() const override;
    int getServerPort() const override;
    QString getUserAgent() const override;
    bool getIgnoreSSLErrors() const override;
    QString language() const;
    Fuoten::FuotenEnums::Type mainViewType() const;
    QDateTime getLastSync() const override;
    Fuoten::FuotenEnums::ItemDeletionStrategy getPerFeedDeletionStrategy(qint64 feedId) const override;
    quint16 getPerFeedDeletionValue(qint64 feedId) const override;

    /*!
     * \brief Getter function for the \link Configuration::updateInterval updateInterval \endlink property.
     * \sa setUpdateInterval(), updateIntervalChanged()
     */
    quint32 updateInterval() const;

    /*!
     * \brief Getter function for the \link Configuration::humanLastSync humanLastSync \endlink property.
     * \sa setHumanLastSync(), humanLastSyncChanged()
     */
    QString humanLastSync() const;

    /*!
     * \brief Getter function for the \link Configuration::wlanOnlyUpdate wlanOnlyUpdate \endlink property.
     * \sa setWlanOnlyUpdate(), wlanOnlyUpdateChanged()
     */
    bool wlanOnlyUpdate() const;

    /*!
     * \brief Getter function for the \link Configuration::articleFontSize articleFontSize \endlink property.
     * \sa setArticleFontSize(), articleFontSizeChanged()
     */
    int articleFontSize() const;

    /*!
     * \brief Getter function for the \link Configuration::notificationsEnabled notificationsEnabled \endlink property.
     * \sa setNotificationsEnabled(), notificationsEnabledChanged()
     */
    bool notificationsEnabled() const;

    /*!
     * \brief Getter function for the \linnk Configuration::navBackAfterMark navBackAfterMark\endlink property.
     * \sa setNavBackAfterMark(), navBackAfterMarkChanged()
     */
    bool navBackAfterMark() const;

    /*!
     * \brief Getter function for the \linnk Configuration::jsSupport jsSupport\endlink property.
     * \sa setJsSupport(), jsSupportChanged()
     */
    FuotenAppEnums::JsSupport jsSupport() const;

    /*!
     * \brief Getter function for the \linnk Configuration::cookieBehavior cookieBehavior\endlink property.
     * \sa setCookieBehavior(), cookieBehaviorChanged()
     */
    FuotenAppEnums::CookieBehavior cookieBehavior() const;

    /*!
     * \brief Returns a human readable relative last sync time.
     */
    Q_INVOKABLE QString getHumanLastSync() const;

    void setUsername(const QString &username) override;
    void setPassword(const QString &password) override;
    void setUseSSL(bool useSSL) override;
    void setHost(const QString &host) override;
    void setInstallPath(const QString &installPath) override;
    void setServerPort(int serverPort) override;
    void setIgnoreSSLErrors(bool ignoreSSLErrors);
    void setLanguage(const QString &nLanguage);
    void setMainViewType(Fuoten::FuotenEnums::Type nMainViewType);
    void setLastSync(const QDateTime &lastSync) override;

    /*!
     * \brief Setter function for the \link Configuration::updateInterval updateInterval \endlink property.
     * \sa setUpdateInterval(), updateIntervalChanged()
     */
    void setUpdateInterval(quint32 updateInterval);

    /*!
     * \brief Setter function for the \link Configuration::wlanOnlyUpdate wlanOnlyUpdate \endlink property.
     * \sa wlanOnlyUpdate(), wlanOnlyUpdateChanged()
     */
    void setWlanOnlyUpdate(bool wlanOnlyUpdate);

    /*!
     * \brief Setter function for the \link Configuration::articleFontSize articleFontSize \endlink property.
     * \sa articleFontSize(), articleFontSizeChanged()
     */
    void setArticleFontSize(int articleFontSize);

    /*!
     * \brief Setter function for the \link Configuration::notificationsEnabled notificationsEnabled \endlink property.
     * \sa notificationsEnabled(), notificationsEnabledChanged()
     */
    void setNotificationsEnabled(bool notificationsEnabled);

    /*!
     * \brief Setter function for the \link Configuration::navBackAfterMark navBackAfterMark\endlink property.
     * \sa navBackAfterMark(), navBackAfterMarkChanged()
     */
    void setNavBackAfterMark(bool navBackAfterMark);

    /*!
     * \brief Setter function for the \link Configuration::jsSupport jsSupport\endlink property.
     * \sa jsSupport(), jsSupportChanged()
     */
    void setJsSupport(FuotenAppEnums::JsSupport jsSupport);

    /*!
     * \brief Setter function for the \link Configuration::cookieBehavior cookieBehavior\endlink property.
     * \sa cookieBehavior(), cookieBehaviorChanged()
     */
    void setCookieBehavior(FuotenAppEnums::CookieBehavior cookieBehavior);

    /*!
     * \brief Returns true if Fuoten has been updated.
     *
     * Compares the current version with the last saved version.
     */
    Q_INVOKABLE bool checkForUpdate() const;

    /*!
     * \brief Returns true if this is the first start of Fuoten.
     *
     * Checks if a valid version has been saved.
     */
    Q_INVOKABLE bool isFirstStart() const;

    /*!
     * \brief Saves the current application version in the settings file.
     */
    Q_INVOKABLE void setCurrentVersion();

    /*!
     * \brief Returns \c true if an update/synchronization is possible.
     *
     * An update is possible if the \link Configuration::updateInterval updateInterval \endlink
     * is not disabled (greater than \c 0) and if the time between now and the last update
     * is greater than the update interval inseconds.
     */
    Q_INVOKABLE bool isUpdatePossible() const;

    /*!
     * \brief Checks if an update/synchrinization should be performed.
     *
     * Starts a 5000ms timer that will use isUpdatePossible() after timeout. If that returns
     * \c true, the updatePossible() signal will be emitted. Will also set the
     * \link Configuration::humanLastSync humanLastSync \endlink property.
     */
    Q_INVOKABLE void checkUpdate();

    /*!
     * \brief Deletes all local account data.
     *
     * This will delete the configuration entries for username, password, host, path,
     * port and current version. After deletion Fuoten::AbstractConfiguration::accountDeleted()
     * will be emitted.
     */
    Q_INVOKABLE void deleteAccount() override;

protected:
    void setIsAccountValid(bool nIsAccountValid) override;

    void setValue(const QString &key, const QVariant &value);

    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

    void remove(const QString &key);

signals:
    void usernameChanged(const QString &username);
    void passwordChanged(const QString &password);
    void useSSLChanged(bool useSSL);
    void hostChanged(const QString &host);
    void installPathChanged(const QString &installPath);
    void isAccountValidChanged(bool isAccountValid);
    void serverPortChanged(int serverPort);
    void ignoreSSLErrorsChanged(bool ignoreSSLErrors);
    void languageChanged(const QString &language);
    void mainViewTypeChanged(Fuoten::FuotenEnums::Type mainViewType);
    void lastSyncChanged(const QDateTime &lastSync);

    /*!
     * \brief Notifier signal for the \link Configuration::updateInterval updateInterval \endlink property.
     * \sa updateInterval(), updateIntervalChanged()
     */
    void updateIntervalChanged(quint32 interval);

    /*!
     * \brief Notifier signal for the \link Configuration::humanLastSync humanLastSync \endlink property.
     * \sa setHumanLastSync(), humanLastSync()
     */
    void humanLastSyncChanged(const QString &humanLastSync);

    /*!
     * \brief Notifier signal for the \link Configuration::wlanOnlyUpdate wlanOnlyUpdate \endlink property.
     * \sa setWlanOnlyUpdate(), wlanOnlyUpdate()
     */
    void wlanOnlyUpdateChanged(bool wlanOnlyUpdate);

    /*!
     * \brief Notifier signal for the \link Configuration::articleFontSize articleFontSize \endlink property.
     * \sa setArticleFontSize(), articleFontSize()
     */
    void articleFontSizeChanged(int articleFontSize);

    /*!
     * \brief Notification signal for the \link Configuration::notificationsEnabled notificationsEnabled \endlink property.
     * \sa notificationsEnabled(), setNotificationsEnabled()
     */
    void notificationsEnabledChanged(bool notificationsEnabled);

    /*!
     * \brief Notification signal for the \link Configuration::navBackAfterMark navBackAfterMark\endlink property.
     * \sa navBackAfterMark(), setNavBackAfterMark()
     */
    void navBackAfterMarkChanged(bool navBackAfterMark);

    /*!
     * \brief Notification signal for the \link Configuration::jsSupport jsSupport\endlink property.
     * \sa jsSupport(), setJsSupport()
     */
    void jsSupportChanged(FuotenAppEnums::JsSupport jsSupport);

    /*!
     * \brief Notification signal for the \link Configuration::cookieBehavior cookieBehavior\endlink property.
     * \sa cookieBehavior(), setCookieBehavior()
     */
    void cookieBehaviorChanged(FuotenAppEnums::CookieBehavior cookieBehavior);

    /*!
     * \brief This signal will be emitted by the checkUpdate() method.
     */
    void updatePossible();

private:
    Q_DISABLE_COPY(Configuration)
    Q_DISABLE_MOVE(Configuration)
    QString m_username;
    QString m_password;
    QString m_host;
    QString m_installPath;
    QString m_language;
    QString m_humanLastSync;
    QDateTime m_lastSync;
    QTimer *m_checkUpdateTimer = nullptr;
    QSettings *m_settings = nullptr;
    QVersionNumber m_savedAppVersion;
    quint32 m_updateInterval = 0;
    int m_serverPort;
    int m_articleFontSize = 0;
    Fuoten::FuotenEnums::Type m_mainViewType;
    FuotenAppEnums::JsSupport m_jsSupport = FuotenAppEnums::JsEnabled;
    FuotenAppEnums::CookieBehavior m_cookieBehavior = FuotenAppEnums::CookiesAcceptAll;
    bool m_notificationsEnabled = true;
    bool m_useSSL = true;
    bool m_isAccountValid = false;
    bool m_ignoreSSLErrors = false;
    bool m_wlanOnlyUpdate = true;
    bool m_navBackAfterMark = true;

    /*!
     * \brief Setter function for the \link Configuration::humanLastSync humanLastSync \endlink property.
     * \sa humanLastSync(), humanLastSyncChanged()
     */
    void setHumanLastSync(const QString &humanLastSync);
};

#endif // CONFIGURATION_H
