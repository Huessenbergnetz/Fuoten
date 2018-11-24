/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/src/main.cpp
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

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#ifdef QT_DEBUG
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#endif

#include <cmath>

#include <QtQml>
#include <QGuiApplication>
#include <QQuickView>
#include <QLocale>
#include <QTranslator>
#include <QDir>
#include <QStandardPaths>
#include <QNetworkDiskCache>
#include <QFile>
#include <QTextStream>
#include <QStringBuilder>

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingCall>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QDBusError>

#ifndef CLAZY
#include <sailfishapp.h>
#include <silicatheme.h>
#include <silicascreen.h>
#endif

#include <Fuoten/Error>
#include <Fuoten/FuotenEnums>
#include <Fuoten/Helpers/AccountValidator>
#include <Fuoten/Helpers/AbstractConfiguration>
#include <Fuoten/Helpers/Synchronizer>
#include <Fuoten/Storage/SQLiteStorage>
#include <Fuoten/Storage/AbstractStorage>
#include <Fuoten/Models/FolderListFilterModel>
#include <Fuoten/API/Component>
#include <Fuoten/API/CreateFolder>
#include <Fuoten/Folder>
#include <Fuoten/Feed>
#include <Fuoten/Article>
#include <Fuoten/API/CreateFeed>
#include <Fuoten/API/GetItems>

#include <Fuoten/Models/FeedListModel>
#include <Fuoten/Models/FeedListFilterModel>
#include <Fuoten/Models/ArticleListModel>
#include <Fuoten/Models/ArticleListFilterModel>

#include "../../common/languagemodel.h"
#include "../../common/enums.h"
#include "../../common/contextconfig.h"
#include "../../common/updateintervalmodel.h"

#include "fuoteniconprovider.h"
#include "hbnsciconprovider.h"
#include "sharing/sharingmethodsmodel.h"
#include "dbus/fuotendbusadaptor.h"
#include "dbus/fuotendbusproxy.h"
#include "namfactory.h"
#include "coverconnector.h"
#include "useragentmodel.h"
#include "sfosconfig.h"
#include "sfosnotificator.h"

void fuotenMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QChar t;
    switch (type) {
    case QtDebugMsg:
        t = QLatin1Char('D');
        break;
    case QtInfoMsg:
        t = QLatin1Char('I');
        break;
    case QtWarningMsg:
        t = QLatin1Char('W');
        break;
    case QtCriticalMsg:
        t = QLatin1Char('C');
        break;
    case QtFatalMsg:
        t = QLatin1Char('F');
        break;
    }


#ifdef QT_DEBUG
    QString txt;

    if (context.function) {

        QRegularExpression re(QStringLiteral("([\\w:]+)\\("));

        txt = QStringLiteral("[%1] %2: %3:%4 - %5").arg(QString(t),
                                                        QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss:zzz")),
                                                        re.match(QString(context.function)).captured(1),
                                                        QString::number(context.line),
                                                        msg);

    } else {
        txt = QStringLiteral("[%1] %2: %3").arg(QString(t), QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss:zzz")), msg);
    }

    fprintf(stderr, "%s\n", qUtf8Printable(txt));

    QFile logFile(QDir::homePath().append(QStringLiteral("/fuoten.log")));
    logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&logFile);
    ts << txt << endl;
#else
    Q_UNUSED(context)
    fprintf(stderr, "[%c] %s: %s\n", t.toLatin1(), qUtf8Printable(QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss:zzz"))), qUtf8Printable(msg));
#endif

    if (type == QtFatalMsg) {
        abort();
    }
}



int main(int argc, char *argv[])
{
#ifndef CLAZY
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
#else
    QScopedPointer<QGuiApplication> app(new QGuiApplication(argc, argv));
#endif

    app->setApplicationName(QStringLiteral("harbour-fuoten"));
    app->setApplicationDisplayName(QStringLiteral("Fuoten"));
    app->setApplicationVersion(QStringLiteral(VERSION_STRING));

#ifdef QT_DEBUG
    QFile::remove(QDir::home().absoluteFilePath(QStringLiteral("fuoten.log")));
#endif
    qInstallMessageHandler(fuotenMessageHandler);

    auto config = new SfosConfig(app.data());

    if (!config->language().isEmpty()) {
        QLocale::setDefault(QLocale(config->language()));
    } else {
        QLocale::setDefault(QLocale::system());
    }



    {
#ifndef CLAZY
        const QString l10nDir = SailfishApp::pathTo(QStringLiteral("translations")).toString(QUrl::RemoveScheme);
#else
        const QString l10nDir;
#endif
        const QLocale locale;
        for (const QString &name : {QStringLiteral("fuoten"), QStringLiteral("libfuoten"), QStringLiteral("hbnsc")}) {
            auto trans = new QTranslator(app.data());
            if (Q_LIKELY(trans->load(locale, name, QStringLiteral("_"), l10nDir, QStringLiteral(".qm")))) {
                if (Q_UNLIKELY(!app->installTranslator(trans))) {
                    qWarning("Can not install translator for component \"%s\" and locale \"%s\".", qUtf8Printable(name), qUtf8Printable(locale.name()));
                }
            } else {
                qWarning("Can not load translations for component \"%s\" and locale \"%s\".", qUtf8Printable(name), qUtf8Printable(locale.name()));
            }
        }

        QTranslator *tfeTrans = new QTranslator(app.data());
        if (locale.language() == QLocale::C) {

            if (Q_LIKELY(tfeTrans->load(QStringLiteral("sailfish_transferengine_plugins_eng_en"), QStringLiteral("/usr/share/translations")))) {
                if (Q_UNLIKELY(!app->installTranslator(tfeTrans))) {
                    qWarning("%s", "Can not install English translator for sailfish transfer engine plugin.");
                }
            } else {
                qWarning("%s", "Can not load English translations for sailfish transfer engine plugin.");
            }

        } else {

            if (Q_LIKELY(tfeTrans->load(locale, QStringLiteral("sailfish_transferengine_plugins"), QStringLiteral("-"), QStringLiteral("/usr/share/translations"), QStringLiteral(".qm")))) {
                if (Q_UNLIKELY(!app->installTranslator(tfeTrans))) {
                    qWarning("Can not install translator for sailfish transfer engine plugin and locale \"%s\".", qUtf8Printable(locale.name()));
                }
            } else {
                qWarning("Can not load translation for sailfish transfer engine plugin and locale \"%s\".", qUtf8Printable(locale.name()));
            }
        }
    }

    QString iconsDir;
    {
#ifndef CLAZY
        const qreal pixelRatio = Silica::Theme::instance()->pixelRatio();
//        const bool largeScreen = Silica::Screen::instance()->sizeCategory() >= Silica::Screen::Large;
        const bool largeScreen = false;
#else
        const qreal pixelRatio = 1.0;
        const bool largeScreen = false;
        Q_UNUSED(largeScreen)
#endif

        qreal nearestScale = 1.0;
        qreal lastDiff = 999.0;
        for (const qreal currentScale : {1.0, 1.25, 1.5, 1.75, 2.0}) {
            const qreal diff = std::abs(currentScale - pixelRatio);
            if (diff < lastDiff) {
                nearestScale = currentScale;
                lastDiff = diff;
            }
            if (lastDiff == 0.0) {
                break;
            }
        }
#ifndef CLAZY
        iconsDir = SailfishApp::pathTo(QStringLiteral("icons/z")).toString(QUrl::RemoveScheme|QUrl::StripTrailingSlash) % QString::number(nearestScale) % (largeScreen ? QStringLiteral("-large") : QStringLiteral("/"));
#endif
    }
    qDebug("Loading icons from %s", qUtf8Printable(iconsDir));

    qRegisterMetaType<Fuoten::IdList>("Fuoten::IdList");
    qRegisterMetaType<Fuoten::ArticleList>("Fuoten::ArticleList");

    auto notificator = new SfosNotificator(config, iconsDir, app.data());

    QObject::connect(app.data(), &QGuiApplication::applicationStateChanged, notificator, [notificator](Qt::ApplicationState state) {
        notificator->setEnabled(state != Qt::ApplicationActive);
    });

    QNetworkDiskCache *qmlDiskCache = nullptr;
    Fuoten::SQLiteStorage *sqliteStorage = nullptr;
    {
        QDir dbusDir(QDir::homePath() + QStringLiteral("/.local/share/dbus-1/services"));
        QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
        QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
        QDir qmlCacheDir(cacheDir.absoluteFilePath(QStringLiteral("qmlcache")));

        if (Q_UNLIKELY(!dbusDir.exists())) {
            if (!dbusDir.mkpath(dbusDir.absolutePath())) {
                //% "Failed to create user D-Bus directory."
                notificator->notify(Fuoten::AbstractNotificator::StorageError, QtFatalMsg, qtTrId("fuoten-fatal-error-failed-dbus-dir"));
                qFatal("Failed to create D-Bus direcotry.");
            }
        }

        QFile dbusFile(dbusDir.absoluteFilePath(QStringLiteral("org.harbour.fuoten.service")));
        if (Q_UNLIKELY(!dbusFile.exists())) {
            if (Q_UNLIKELY(!dbusFile.open(QIODevice::WriteOnly|QIODevice::Text))) {
                //% "Failed to open D-Bus service file for writing."
                notificator->notify(Fuoten::AbstractNotificator::ApplicationError, QtFatalMsg, qtTrId("fuoten-fatal-error-failed-dbus-file"));
                qFatal("Failed to open D-Bus service file for writing.");
            }
            QTextStream dbusOut(&dbusFile);
            dbusOut << QStringLiteral("[D-BUS Service]\n");
            dbusOut << QStringLiteral("Name=org.harbour.fuoten\n");
            dbusOut << QStringLiteral("Exec=/usr/bin/invoker -s --type=silica-qt5 /usr/bin/harbour-fuoten");
            dbusOut.flush();
            dbusFile.close();

            QDBusConnection dc = QDBusConnection::sessionBus();
            if (!dc.isConnected()) {
                //% "Failed to connect to D-Bus session bus."
                notificator->notify(Fuoten::AbstractNotificator::ApplicationError, QtFatalMsg, qtTrId("fuoten-fatal-error-failed-dbus-connection"));
                qFatal("Failed to connect to D-Bus session bus.");
            }
            QDBusMessage m = QDBusMessage::createMethodCall(QStringLiteral("org.freedesktop.DBus"), QStringLiteral("/"), QStringLiteral("org.freedesktop.DBus"), QStringLiteral("ReloadConfig"));
            m.setAutoStartService(false);
            QDBusPendingCall pc = dc.asyncCall(m, 3000);
            auto pcw = new QDBusPendingCallWatcher(pc, app.data());
            QObject::connect(pcw, &QDBusPendingCallWatcher::finished, notificator, [notificator](QDBusPendingCallWatcher *call){
                QDBusPendingReply<void> reply = *call;
                if (reply.isError()) {
                    //% "Failed to reload D-Bus session bus configuration."
                    notificator->notify(Fuoten::AbstractNotificator::ApplicationError, QtFatalMsg, qtTrId("fuoten-fatal-error-failed-dbus-reload-config"));
                    qCritical("D-Bus error: %s (%s)", qUtf8Printable(reply.error().message()), qUtf8Printable(reply.error().name()));
                    qFatal("Failed to reload D-Bus session bus configuration.");
                }
                call->deleteLater();
            });
        }

        if (Q_UNLIKELY(!dataDir.exists())) {
            if (!dataDir.mkpath(dataDir.absolutePath())) {
                //% "Failed to create the data directory."
                notificator->notify(Fuoten::AbstractNotificator::StorageError, QtFatalMsg, qtTrId("fuoten-fatal-error-failed-data-dir"));
                qFatal("Failed to create data directory.");
            }
        }

        if (Q_UNLIKELY(!cacheDir.exists())) {
            if (!cacheDir.mkpath(cacheDir.absolutePath())) {
                //% "Failed to create the cache directory."
                notificator->notify(Fuoten::AbstractNotificator::StorageError, QtFatalMsg, qtTrId("fuoten-fatal-error-failed-cache-dir"));
                qFatal("Failed to create cache directory.");
            }
        }

        if (Q_UNLIKELY(!qmlCacheDir.exists())) {
            if (!qmlCacheDir.mkpath(qmlCacheDir.absolutePath())) {
                //% "Failed to create the qml cache directory."
                notificator->notify(Fuoten::AbstractNotificator::StorageError, QtFatalMsg, qtTrId("fuoten-fatal-error-failed-qmlcache-dir"));
                qFatal("Failed to create qml cache directory.");
            }
        }

        qmlDiskCache = new QNetworkDiskCache(app.data());
        qmlDiskCache->setCacheDirectory(qmlCacheDir.absolutePath());

        sqliteStorage = new Fuoten::SQLiteStorage(dataDir.absoluteFilePath(QStringLiteral("database.sqlite")), app.data());
        sqliteStorage->setConfiguration(config);
        sqliteStorage->init();
    }
    QScopedPointer<NamFactory> namFactory(new NamFactory(qmlDiskCache));

    Fuoten::Component::setDefaultConfiguration(config);
    Fuoten::Component::setDefaultStorage(sqliteStorage);
    Fuoten::Component::setDefaultNotificator(notificator);

    auto synchronizer = new Fuoten::Synchronizer(app.data());
    synchronizer->setConfiguration(config);
    synchronizer->setStorage(sqliteStorage);

    QObject::connect(config, &Configuration::updatePossible, synchronizer, &Fuoten::Synchronizer::start);

    qmlRegisterUncreatableType<Fuoten::FuotenEnums>("harbour.fuoten", 1, 0, "Fuoten", QStringLiteral("You can not create a Fuoten object"));
    qmlRegisterUncreatableType<Fuoten::AbstractConfiguration>("harbour.fuoten", 1, 0, "FuotenConfiguration", QStringLiteral("You can not create a FuotenConfiguration object."));
    qmlRegisterType<Fuoten::Error>("harbour.fuoten", 1, 0, "FuotenError");
    qmlRegisterType<Fuoten::AccountValidator>("harbour.fuoten", 1, 0, "AccountValidator");
    qmlRegisterUncreatableType<Fuoten::AbstractStorage>("harbour.fuoten", 1, 0, "AbstractStorage", QStringLiteral("You can not create an AbstractStorage object."));

    qmlRegisterType<Fuoten::FolderListFilterModel>("harbour.fuoten.models", 1, 0, "FolderListFilterModel");
    qmlRegisterType<Fuoten::FeedListModel>("harbour.fuoten.models", 1, 0, "FeedListModel");
    qmlRegisterType<Fuoten::FeedListFilterModel>("harbour.fuoten.models", 1, 0, "FeedListFilterModel");
    qmlRegisterType<Fuoten::ArticleListModel>("harbour.fuoten.models", 1, 0, "ArticleListModel");
    qmlRegisterType<Fuoten::ArticleListFilterModel>("harbour.fuoten.models", 1, 0, "ArticleListFilterModel");

    qmlRegisterType<Fuoten::CreateFolder>("harbour.fuoten.api", 1, 0, "CreateFolder");
    qmlRegisterType<Fuoten::CreateFeed>("harbour.fuoten.api", 1, 0, "CreateFeed");
    qmlRegisterType<Fuoten::GetItems>("harbour.fuoten.api", 1, 0, "GetItems");

    qmlRegisterType<Fuoten::Folder>("harbour.fuoten.items", 1, 0, "Folder");
    qmlRegisterType<Fuoten::Feed>("harbour.fuoten.items", 1, 0, "Feed");
    qmlRegisterType<Fuoten::Article>("harbour.fuoten.items", 1, 0, "Article");

    qmlRegisterType<LanguageModel>("harbour.fuoten", 1, 0, "LanguageModel");
    qmlRegisterUncreatableType<Configuration>("harbour.fuoten", 1, 0, "Configuration", QStringLiteral("You can not create a Configuration object"));
    qmlRegisterType<UpdateIntervalModel>("harbour.fuoten", 1, 0, "UpdateIntervalModel");

    qmlRegisterUncreatableType<FuotenAppEnums>("harbour.fuoten", 1, 0, "FuotenApp", QStringLiteral("You can not create a FuotenApp object."));
    qmlRegisterType<ContextConfig>("harbour.fuoten", 1, 0, "ContextConfig");
    qmlRegisterType<SharingMethodsModel>("harbour.fuoten", 1, 0, "SharingMethodsModel");
    qmlRegisterType<UserAgentModel>("harbour.fuoten", 1, 0, "UserAgentModel");

#ifndef CLAZY
    QScopedPointer<QQuickView> view(SailfishApp::createView());
    view->engine()->addImageProvider(QStringLiteral("hbnsc"), new HbnscIconProvider({1.0, 1.25, 1.5, 1.75, 2.0}, Silica::Theme::instance()->pixelRatio(), false));
#else
    QScopedPointer<QQuickView> view(new QQuickView);
#endif
    view->engine()->addImageProvider(QStringLiteral("fuoten"), new FuotenIconProvider(iconsDir));
    view->engine()->setNetworkAccessManagerFactory(namFactory.data());

    auto dbusproxy = new FuotenDbusProxy(app.data());
    new FuotenDbusAdaptor(dbusproxy);
    {
        QDBusConnection con = QDBusConnection::sessionBus();
        if (Q_UNLIKELY(!con.registerService(QStringLiteral("org.harbour.fuoten")))) {
            //% "Failed to register D-Bus service."
            notificator->notify(Fuoten::AbstractNotificator::ApplicationError, QtFatalMsg, qtTrId("fuoten-fatal-error-failed-dbus-service-register"));
            qFatal("Failed to register D-Bus service.");
        }

        if (Q_UNLIKELY(!con.registerObject(QStringLiteral("/"), dbusproxy))) {
            //: %1 will be replaced by the class name of the D-Bus adaptor
            //% "Failed to register D-Bus object “%1”."
            notificator->notify(Fuoten::AbstractNotificator::ApplicationError, QtFatalMsg, qtTrId("fuoten-fatal-error-failed-dbus-object-register").arg(QString::fromLatin1(dbusproxy->metaObject()->className())));
            qFatal("Failed to register D-Bus object \"%s\".", dbusproxy->metaObject()->className());
        }
    }

    view->rootContext()->setContextProperty(QStringLiteral("config"), config);
    view->rootContext()->setContextProperty(QStringLiteral("localstorage"), sqliteStorage);
    view->rootContext()->setContextProperty(QStringLiteral("synchronizer"), synchronizer);
    view->rootContext()->setContextProperty(QStringLiteral("covercon"), new CoverConnector(app.data()));
    view->rootContext()->setContextProperty(QStringLiteral("_fuotenDbusProxy"), dbusproxy);

#ifndef CLAZY
    view->setSource(SailfishApp::pathTo(QStringLiteral("qml/harbour-fuoten.qml")));
#endif

    view->show();

    return app->exec();
}

