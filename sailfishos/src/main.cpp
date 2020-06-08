/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2019 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
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
#include <memory>

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

#include <sailfishapp.h>

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
#include <Fuoten/API/GetServerStatus>
#include <Fuoten/API/LoginFlowV2>
#include <Fuoten/API/ConvertToAppPassword>

#include <Fuoten/Models/FeedListModel>
#include <Fuoten/Models/FeedListFilterModel>
#include <Fuoten/Models/ArticleListModel>
#include <Fuoten/Models/ArticleListFilterModel>

#include "../../common/enums.h"
#include "../../common/contextconfig.h"
#include "../../common/updateintervalmodel.h"

#include "hbnsciconprovider.h"
#include "sharing/sharingmethodsmodel.h"
#include "dbus/fuotendbusadaptor.h"
#include "dbus/fuotendbusproxy.h"
#include "namfactory.h"
#include "coverconnector.h"
#include "useragentmodel.h"
#include "sfosconfig.h"
#include "sfosnotificator.h"
#include "languagesmodel.h"
#include "licensesmodel.h"

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
    std::unique_ptr<QGuiApplication> app(SailfishApp::application(argc, argv));

    app->setApplicationName(QStringLiteral("harbour-fuoten"));
    app->setApplicationDisplayName(QStringLiteral("Fuoten"));
    app->setApplicationVersion(QStringLiteral(VERSION_STRING));

#ifdef QT_DEBUG
    QFile::remove(QDir::home().absoluteFilePath(QStringLiteral("fuoten.log")));
#endif
    qInstallMessageHandler(fuotenMessageHandler);

    auto config = new SfosConfig(app.get());

    if (!config->language().isEmpty()) {
        QLocale::setDefault(QLocale(config->language()));
    }

    {
        const QLocale locale;
        for (const QString &name : {QStringLiteral("fuoten"), QStringLiteral("libfuoten"), QStringLiteral("hbnsc")}) {
            auto trans = new QTranslator(app.get());
            if (Q_LIKELY(trans->load(locale, name, QStringLiteral("_"), QStringLiteral(TRANSLATIONS_DIR), QStringLiteral(".qm")))) {
                if (Q_UNLIKELY(!app->installTranslator(trans))) {
                    qWarning(R"(Can not install translator for component "%s" and locale "%s".)", qUtf8Printable(name), qUtf8Printable(locale.name()));
                }
            } else {
                qWarning(R"(Can not load translations for component "%s" and locale "%s".)", qUtf8Printable(name), qUtf8Printable(locale.name()));
            }
        }

        auto tfeTrans = new QTranslator(app.get());
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

    qRegisterMetaType<Fuoten::IdList>("Fuoten::IdList");
    qRegisterMetaType<Fuoten::ArticleList>("Fuoten::ArticleList");

    auto notificator = new SfosNotificator(config, app.get());

    QObject::connect(app.get(), &QGuiApplication::applicationStateChanged, notificator, [notificator](Qt::ApplicationState state) {
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
                //: error message
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
                //: error message
                //% "Failed to connect to D-Bus session bus."
                notificator->notify(Fuoten::AbstractNotificator::ApplicationError, QtFatalMsg, qtTrId("fuoten-fatal-error-failed-dbus-connection"));
                qFatal("Failed to connect to D-Bus session bus.");
            }
            QDBusMessage m = QDBusMessage::createMethodCall(QStringLiteral("org.freedesktop.DBus"), QStringLiteral("/"), QStringLiteral("org.freedesktop.DBus"), QStringLiteral("ReloadConfig"));
            m.setAutoStartService(false);
            QDBusPendingCall pc = dc.asyncCall(m, 3000);
            auto pcw = new QDBusPendingCallWatcher(pc, app.get());
            QObject::connect(pcw, &QDBusPendingCallWatcher::finished, notificator, [notificator](QDBusPendingCallWatcher *call){
                QDBusPendingReply<void> reply = *call;
                if (reply.isError()) {
                    //: error message
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
                //: error message
                //% "Failed to create the data directory."
                notificator->notify(Fuoten::AbstractNotificator::StorageError, QtFatalMsg, qtTrId("fuoten-fatal-error-failed-data-dir"));
                qFatal("Failed to create data directory.");
            }
        }

        if (Q_UNLIKELY(!cacheDir.exists())) {
            if (!cacheDir.mkpath(cacheDir.absolutePath())) {
                //: error message
                //% "Failed to create the cache directory."
                notificator->notify(Fuoten::AbstractNotificator::StorageError, QtFatalMsg, qtTrId("fuoten-fatal-error-failed-cache-dir"));
                qFatal("Failed to create cache directory.");
            }
        }

        if (Q_UNLIKELY(!qmlCacheDir.exists())) {
            if (!qmlCacheDir.mkpath(qmlCacheDir.absolutePath())) {
                //: error message
                //% "Failed to create the qml cache directory."
                notificator->notify(Fuoten::AbstractNotificator::StorageError, QtFatalMsg, qtTrId("fuoten-fatal-error-failed-qmlcache-dir"));
                qFatal("Failed to create qml cache directory.");
            }
        }

        qmlDiskCache = new QNetworkDiskCache(app.get());
        qmlDiskCache->setCacheDirectory(qmlCacheDir.absolutePath());

        sqliteStorage = new Fuoten::SQLiteStorage(dataDir.absoluteFilePath(QStringLiteral("database.sqlite")), app.get());
        sqliteStorage->setConfiguration(config);
        sqliteStorage->init();
    }
    auto namFactory = std::make_unique<NamFactory>(qmlDiskCache);

    Fuoten::Component::setDefaultConfiguration(config);
    Fuoten::Component::setDefaultStorage(sqliteStorage);
    Fuoten::Component::setDefaultNotificator(notificator);

    auto synchronizer = new Fuoten::Synchronizer(app.get());
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
    qmlRegisterType<Fuoten::GetServerStatus>("harbour.fuoten.api", 1, 0, "GetServerStatus");
    qmlRegisterType<Fuoten::LoginFlowV2>("harbour.fuoten.api", 1, 0, "LoginFlowV2");

    qmlRegisterType<Fuoten::Folder>("harbour.fuoten.items", 1, 0, "Folder");
    qmlRegisterType<Fuoten::Feed>("harbour.fuoten.items", 1, 0, "Feed");
    qmlRegisterType<Fuoten::Article>("harbour.fuoten.items", 1, 0, "Article");

    qmlRegisterType<LanguagesModel>("harbour.fuoten", 1, 0, "LanguageModel");
    qmlRegisterType<LicensesModel>("harbour.fuoten", 1, 0, "LicensesModel");
    qmlRegisterUncreatableType<Configuration>("harbour.fuoten", 1, 0, "Configuration", QStringLiteral("You can not create a Configuration object"));
    qmlRegisterType<UpdateIntervalModel>("harbour.fuoten", 1, 0, "UpdateIntervalModel");

    qmlRegisterUncreatableType<FuotenAppEnums>("harbour.fuoten", 1, 0, "FuotenApp", QStringLiteral("You can not create a FuotenApp object."));
    qmlRegisterType<ContextConfig>("harbour.fuoten", 1, 0, "ContextConfig");
    qmlRegisterType<SharingMethodsModel>("harbour.fuoten", 1, 0, "SharingMethodsModel");
    qmlRegisterType<UserAgentModel>("harbour.fuoten", 1, 0, "UserAgentModel");

    std::unique_ptr<QQuickView> view(SailfishApp::createView());

    auto hbnscIconProvider = Hbnsc::HbnscIconProvider::createProvider(view->engine());
    auto fuotenIconProvider = Hbnsc::BaseIconProvider::createProvider({1.0,1.25,1.5,1.75,2.0}, QString(), false, QStringLiteral("fuoten"), view->engine());
    view->engine()->setNetworkAccessManagerFactory(namFactory.get());

    auto dbusproxy = new FuotenDbusProxy(app.get());
    new FuotenDbusAdaptor(dbusproxy);
    {
        QDBusConnection con = QDBusConnection::sessionBus();
        if (Q_UNLIKELY(!con.registerService(QStringLiteral("org.harbour.fuoten")))) {
            //: error message
            //% "Failed to register D-Bus service."
            notificator->notify(Fuoten::AbstractNotificator::ApplicationError, QtFatalMsg, qtTrId("fuoten-fatal-error-failed-dbus-service-register"));
            qFatal("Failed to register D-Bus service.");
        }

        if (Q_UNLIKELY(!con.registerObject(QStringLiteral("/"), dbusproxy))) {
            //: error message, %1 will be replaced by the class name of the D-Bus adaptor
            //% "Failed to register D-Bus object “%1”."
            notificator->notify(Fuoten::AbstractNotificator::ApplicationError, QtFatalMsg, qtTrId("fuoten-fatal-error-failed-dbus-object-register").arg(QString::fromLatin1(dbusproxy->metaObject()->className())));
            qFatal("Failed to register D-Bus object \"%s\".", dbusproxy->metaObject()->className());
        }
    }

    if (config->isAccountValid()) {
        auto appPassConverter = new Fuoten::ConvertToAppPassword(app.get());
        QObject::connect(appPassConverter, &Fuoten::ConvertToAppPassword::succeeded, appPassConverter, &Fuoten::ConvertToAppPassword::deleteLater);
        QObject::connect(appPassConverter, &Fuoten::ConvertToAppPassword::failed, appPassConverter, &Fuoten::ConvertToAppPassword::deleteLater);
        appPassConverter->execute();
    }

    view->rootContext()->setContextProperty(QStringLiteral("config"), config);
    view->rootContext()->setContextProperty(QStringLiteral("localstorage"), sqliteStorage);
    view->rootContext()->setContextProperty(QStringLiteral("synchronizer"), synchronizer);
    view->rootContext()->setContextProperty(QStringLiteral("covercon"), new CoverConnector(app.get()));
    view->rootContext()->setContextProperty(QStringLiteral("_fuotenDbusProxy"), dbusproxy);

    view->setSource(SailfishApp::pathToMainQml());

    view->showFullScreen();

    return app->exec();
}

