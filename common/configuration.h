/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * common/configuration.h
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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QUrl>
#include <Helpers/configuration.h>
#include <fuoten.h>
#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
#include <Helpers/versionnumber.h>
#else
#include <QVersionNumber>
#endif

/*!
 * \brief Provides the configuration interface for Fuoten.
 */
class Configuration : public Fuoten::Configuration
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
     * \brief The display name of the user.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>displayName() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>displayNameChanged(const QString &displayName)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString displayName READ displayName NOTIFY displayNameChanged)
    /*!
     * \brief Returns true if the cron is not configured properly.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>improperlyConfiguredCron() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>improperlyConfiguredCronChanged(bool improperlyConfiguredCron)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool improperlyConfiguredCron READ improperlyConfiguredCron NOTIFY improperlyConfiguredCronChanged)
    /*!
     * \brief Contains the version of the News App on the server.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>serverVersion() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>serverVersionChanged(const QString &serverVersion)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString serverVersion READ serverVersion NOTIFY serverVersionChanged)
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
     * \brief Return the URL to the user avatar image.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QUrl</TD><TD>avatar() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>avatarChanged(const QUrl &avatar)</TD></TR></TABLE>
     */
    Q_PROPERTY(QUrl avatar READ avatar NOTIFY avatarChanged)
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
     * \brief Stores the sorting of the main view.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Fuoten::FuotenEnums::SortingRole</TD><TD>mainViewSorting() const</TD></TR><TR><TD>void</TD><TD>setMainViewSorting(Fuoten::FuotenEnums::SortingRole nMainViewSorting)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>mainViewSortingChanged(Fuoten::FuotenEnums::SortingRole mainViewSorting)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::FuotenEnums::SortingRole mainViewSorting READ mainViewSorting WRITE setMainViewSorting NOTIFY mainViewSortingChanged)
    /*!
     * \brief If true, objects without unread items will be hidden on the main view.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>mainViewHideRead() const</TD></TR><TR><TD>void</TD><TD>setMainViewHideRead(bool nMainViewHideRead)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>mainViewHideReadChanged(bool mainViewHideRead)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool mainViewHideRead READ mainViewHideRead WRITE setMainViewHideRead NOTIFY mainViewHideReadChanged)
    /*!
     * \brief Stores the sort order of the main view content.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Qt::SortOrder</TD><TD>mainViewSortOrder() const</TD></TR><TR><TD>void</TD><TD>setMainViewSortOrder(Qt::SortOrder nMainViewSortOrder)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>mainViewSortOrderChanged(Qt::SortOrder mainViewSortOrder)</TD></TR></TABLE>
     */
    Q_PROPERTY(Qt::SortOrder mainViewSortOrder READ mainViewSortOrder WRITE setMainViewSortOrder NOTIFY mainViewSortOrderChanged)
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_ENUM(Fuoten::FuotenEnums::Type)
    Q_ENUM(Fuoten::FuotenEnums::SortingRole)
#else
    Q_ENUMS(Fuoten::FuotenEnums::Type)
    Q_ENUMS(Fuoten::FuotenEnums::SortingRole)
#endif
public:
    explicit Configuration(QObject *parent = nullptr);
    ~Configuration();

    QString getUsername() const override;
    QString getPassword() const override;
    bool getUseSSL() const override;
    QString getHost() const override;
    QString getInstallPath() const override;
    QString displayName() const;
    bool improperlyConfiguredCron() const;
    bool isAccountValid() const override;
    int getServerPort() const override;
    QString getUserAgent() const override;
    bool getIgnoreSSLErrors() const override;
    QVersionNumber getServerVersion() const override;
    QString serverVersion() const;
    QUrl avatar() const;
    QString language() const;
    Fuoten::FuotenEnums::Type mainViewType() const;
    Fuoten::FuotenEnums::SortingRole mainViewSorting() const;
    bool mainViewHideRead() const;
    Qt::SortOrder mainViewSortOrder() const;

    void setUsername(const QString &username);
    void setPassword(const QString &password);
    void setUseSSL(bool useSSL);
    void setHost(const QString &host);
    void setInstallPath(const QString &installPath);
    void setServerPort(int serverPort);
    void setIgnoreSSLErrors(bool ignoreSSLErrors);
    void setDisplayName(const QString &nDisplayName) override;
    void setImproperlyConfiguredCron(bool nImproperlyConfiguredCron) override;
    void setServerVersion(const QString &nServerVersion) override;
    void setAvatar(const QString &data, const QString &mime) override;
    void setLanguage(const QString &nLanguage);
    void setMainViewType(Fuoten::FuotenEnums::Type nMainViewType);
    void setMainViewSorting(Fuoten::FuotenEnums::SortingRole nMainViewSorting);
    void setMainViewHideRead(bool nMainViewHideRead);
    void setMainViewSortOrder(Qt::SortOrder nMainViewSortOrder);

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

protected:
    void setIsAccountValid(bool nIsAccountValid) override;

signals:
    void usernameChanged(const QString &username);
    void passwordChanged(const QString &password);
    void useSSLChanged(bool useSSL);
    void hostChanged(const QString &host);
    void installPathChanged(const QString &installPath);
    void displayNameChanged(const QString &displayName);
    void improperlyConfiguredCronChanged(bool improperlyConfiguredCron);
    void serverVersionChanged(const QString &serverVersion);
    void isAccountValidChanged(bool isAccountValid);
    void serverPortChanged(int serverPort);
    void ignoreSSLErrorsChanged(bool ignoreSSLErrors);
    void avatarChanged(const QUrl &avatar);
    void languageChanged(const QString &language);
    void mainViewTypeChanged(Fuoten::FuotenEnums::Type mainViewType);
    void mainViewSortingChanged(Fuoten::FuotenEnums::SortingRole mainViewSorting);
    void mainViewHideReadChanged(bool mainViewHideRead);
    void mainViewSortOrderChanged(Qt::SortOrder mainViewSortOrder);

private:
    Q_DISABLE_COPY(Configuration)
    QString m_username;
    QString m_password;
    bool m_useSSL;
    QString m_host;
    QString m_installPath;
    QString m_displayName;
    bool m_improperlyConfiguredCron;
    QVersionNumber m_serverVersion;
    QVersionNumber m_savedAppVersion;
    bool m_isAccountValid;
    int m_serverPort;
    bool m_ignoreSSLErrors;
    QUrl m_avatar;
    QString m_language;
    Fuoten::FuotenEnums::Type m_mainViewType;
    Fuoten::FuotenEnums::SortingRole m_mainViewSorting;
    bool m_mainViewHideRead;
    Qt::SortOrder m_mainViewSortOrder;
};

#endif // CONFIGURATION_H
