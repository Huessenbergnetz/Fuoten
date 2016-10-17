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
    Fuoten::Configuration(parent)
{
    m_username = value(QStringLiteral("account/user")).toString();
    m_password = value(QStringLiteral("account/password")).toString();
    m_useSSL = value(QStringLiteral("account/usessl"), true).toBool();
    m_host = value(QStringLiteral("account/host")).toString();
    m_installPath = value(QStringLiteral("account/installpath")).toString();
    m_displayName = value(QStringLiteral("account/displayname")).toString();
#if QT_VERSION < QT_VERSION_CHECK(5, 6 ,0)
    m_serverVersion = Fuoten::NewsAppVersion(value(QStringLiteral("account/serverversion")).toString());
#else
    m_serverVersion = QVersionNumber::fromString(value(QStringLiteral("account/serverversion")).toString());
#endif
    m_serverPort = value(QStringLiteral("account/serverport"), 0).toInt();
    m_improperlyConfiguredCron = value(QStringLiteral("warnings/improperlyConfiguredCron"), false).toBool();
    m_isAccountValid = false;
    checkAccountValidity();
    m_ignoreSSLErrors = value(QStringLiteral("account/ignoresslerrors"), false).toBool();
    m_avatar = value(QStringLiteral("account/avatar"), QStringLiteral(DEFAULT_AVATAR)).toUrl();
    m_language = value(QStringLiteral("display/language")).toString();
    m_mainViewType = (Fuoten::Fuoten::Type)value(QStringLiteral("display/mainViewType"), Fuoten::Fuoten::Feed).toInt();
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


#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
QVersionNumber Configuration::getServerVersion() const { return m_serverVersion; }
#else
Fuoten::NewsAppVersion Configuration::getServerVersion() const { return m_serverVersion; }
#endif

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




Fuoten::Fuoten::Type Configuration::mainViewType() const { return m_mainViewType; }

void Configuration::setMainViewType(Fuoten::Fuoten::Type nMainViewType)
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

