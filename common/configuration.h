#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <Helpers/configuration.h>
#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
#include <Helpers/newsappversion.h>
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
public:
    explicit Configuration(QObject *parent = nullptr);
    ~Configuration();

    QString getUsername() const Q_DECL_OVERRIDE;
    QString getPassword() const Q_DECL_OVERRIDE;
    bool getUseSSL() const Q_DECL_OVERRIDE;
    QString getHost() const Q_DECL_OVERRIDE;
    QString getInstallPath() const Q_DECL_OVERRIDE;
    QString displayName() const;
    bool improperlyConfiguredCron() const;
    bool isAccountValid() const Q_DECL_OVERRIDE;
    int getServerPort() const Q_DECL_OVERRIDE;
    QString getUserAgent() const Q_DECL_OVERRIDE;
    bool getIgnoreSSLErrors() const Q_DECL_OVERRIDE;
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QVersionNumber getServerVersion() const Q_DECL_OVERRIDE;
#else
    Fuoten::NewsAppVersion getServerVersion() const Q_DECL_OVERRIDE;
#endif
    QString serverVersion() const;

    void setUsername(const QString &username);
    void setPassword(const QString &password);
    void setUseSSL(bool useSSL);
    void setHost(const QString &host);
    void setInstallPath(const QString &installPath);
    void setServerPort(int serverPort);
    void setIgnoreSSLErrors(bool ignoreSSLErrors);

protected:
    void setDisplayName(const QString &nDisplayName) Q_DECL_OVERRIDE;
    void setImproperlyConfiguredCron(bool nImproperlyConfiguredCron) Q_DECL_OVERRIDE;
    void setServerVersion(const QString &nServerVersion) Q_DECL_OVERRIDE;
    void setIsAccountValid(bool nIsAccountValid) Q_DECL_OVERRIDE;

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

private:
    Q_DISABLE_COPY(Configuration)
    QString m_username;
    QString m_password;
    bool m_useSSL;
    QString m_host;
    QString m_installPath;
    QString m_displayName;
    bool m_improperlyConfiguredCron;
#if QT_VERSION < QT_VERSION_CHECK(5, 6 ,0)
    Fuoten::NewsAppVersion m_serverVersion;
#else
    QVersionNumber m_serverVersion;
#endif
    bool m_isAccountValid;
    int m_serverPort;
    bool m_ignoreSSLErrors;
};

#endif // CONFIGURATION_H
