/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
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

#include <sailfishapp.h>

#include <Fuoten/Error>
#include <Fuoten/FuotenEnums>
#include <Fuoten/Helpers/AbstractConfiguration>
#include <Fuoten/Helpers/Synchronizer>
#include <Fuoten/Helpers/WipeManager>
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
#include <Fuoten/API/GetStatus>
#include <Fuoten/API/DeleteAppPassword>

#include <Fuoten/Models/FeedListModel>
#include <Fuoten/Models/FeedListFilterModel>
#include <Fuoten/Models/ArticleListModel>
#include <Fuoten/Models/ArticleListFilterModel>

#include <hbnsciconprovider.h>

#include "enums.h"
#include "updateintervalmodel.h"

#include "dbus/fuotendbusadaptor.h"
#include "dbus/fuotendbusproxy.h"
#include "namfactory.h"
#include "coverconnector.h"
#include "useragentmodel.h"
#include "sfosconfig.h"
#include "sfosnotificator.h"
#include "sfosuseravatar.h"
#include "languagesmodel.h"
#include "licensesmodel.h"
#include "sfosmigrator.h"
#include "sfoscontextconfig.h"

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

    app->setApplicationName(QStringLiteral("fuoten"));
    app->setApplicationDisplayName(QStringLiteral("Fuoten"));
    app->setOrganizationName(QStringLiteral("de.huessenbergnetz"));
    app->setApplicationVersion(QStringLiteral(VERSION_STRING));

#ifdef QT_DEBUG
    QFile::remove(QDir::home().absoluteFilePath(QStringLiteral("fuoten.log")));
#endif
    qInstallMessageHandler(fuotenMessageHandler);

    if (Q_UNLIKELY(!SfosMigrator::migrate())) {
        qFatal("%s", "Failed to migrate application data and configuration");
    }

    auto config = new SfosConfig(app.get());

    if (!config->language().isEmpty()) {
        QLocale::setDefault(QLocale(config->language()));
    }

    {
        const QLocale locale;
        for (const QString &name : {QStringLiteral("fuoten"), QStringLiteral("libfuoten"), QStringLiteral("hbnsc")}) {
            auto trans = new QTranslator(app.get());
            if (Q_LIKELY(trans->load(locale, name, QStringLiteral("_"), QStringLiteral(FUOTEN_I18NDIR), QStringLiteral(".qm")))) {
                if (Q_UNLIKELY(!app->installTranslator(trans))) {
                    qWarning(R"(Can not install translator for component "%s" and locale "%s".)", qUtf8Printable(name), qUtf8Printable(locale.name()));
                }
            } else {
                qWarning(R"(Can not load translations for component "%s" and locale "%s" from "%s".)", qUtf8Printable(name), qUtf8Printable(locale.name()), FUOTEN_I18NDIR);
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
        QDir dataDir(SfosMigrator::dataDirPath());
        QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
        QDir qmlCacheDir(cacheDir.absoluteFilePath(QStringLiteral("qmlcache")));

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

    auto wipeManager = new Fuoten::WipeManager(app.get());
    wipeManager->setConfiguration(config);
    wipeManager->setStorage(sqliteStorage);
    wipeManager->setNotificator(notificator);

    Fuoten::Component::setDefaultConfiguration(config);
    Fuoten::Component::setDefaultStorage(sqliteStorage);
    Fuoten::Component::setDefaultNotificator(notificator);
    Fuoten::Component::setDefaultWipeManager(wipeManager);

    auto synchronizer = new Fuoten::Synchronizer(app.get());
    synchronizer->setConfiguration(config);
    synchronizer->setStorage(sqliteStorage);

    QObject::connect(config, &Configuration::updatePossible, synchronizer, &Fuoten::Synchronizer::start);

    qmlRegisterUncreatableType<Fuoten::FuotenEnums>("harbour.fuoten", 1, 0, "Fuoten", QStringLiteral("You can not create a Fuoten object"));
    qmlRegisterUncreatableType<Fuoten::AbstractConfiguration>("harbour.fuoten", 1, 0, "FuotenConfiguration", QStringLiteral("You can not create a FuotenConfiguration object."));
    qmlRegisterType<Fuoten::Error>("harbour.fuoten", 1, 0, "FuotenError");
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
    qmlRegisterType<Fuoten::GetStatus>("harbour.fuoten.api", 1, 0, "GetStatus");
    qmlRegisterType<Fuoten::DeleteAppPassword>("harbour.fuoten.api", 1, 0, "DeleteAppPassword");

    qmlRegisterType<Fuoten::Folder>("harbour.fuoten.items", 1, 0, "Folder");
    qmlRegisterType<Fuoten::Feed>("harbour.fuoten.items", 1, 0, "Feed");
    qmlRegisterType<Fuoten::Article>("harbour.fuoten.items", 1, 0, "Article");

    qmlRegisterType<LanguagesModel>("harbour.fuoten", 1, 0, "LanguageModel");
    qmlRegisterType<LicensesModel>("harbour.fuoten", 1, 0, "LicensesModel");
    qmlRegisterUncreatableType<Configuration>("harbour.fuoten", 1, 0, "Configuration", QStringLiteral("You can not create a Configuration object"));
    qmlRegisterType<UpdateIntervalModel>("harbour.fuoten", 1, 0, "UpdateIntervalModel");

    qmlRegisterUncreatableType<FuotenAppEnums>("harbour.fuoten", 1, 0, "FuotenApp", QStringLiteral("You can not create a FuotenApp object."));
    qmlRegisterType<SfosContextConfig>("harbour.fuoten", 1, 0, "ContextConfig");
    qmlRegisterType<UserAgentModel>("harbour.fuoten", 1, 0, "UserAgentModel");
    qmlRegisterType<SfosUserAvatar>("harbour.fuoten", 1, 0, "UserAvatar");

    std::unique_ptr<QQuickView> view(SailfishApp::createView());

    auto hbnscIconProvider = Hbnsc::HbnscIconProvider::createProvider(view->engine());
    auto fuotenIconProvider = Hbnsc::BaseIconProvider::createProvider({1.0,1.25,1.5,1.75,2.0}, QString(), false, QStringLiteral("fuoten"), view->engine());
    view->engine()->setNetworkAccessManagerFactory(namFactory.get());

    auto dbusproxy = new FuotenDbusProxy(app.get());
    new FuotenDbusAdaptor(dbusproxy);
    {
        QDBusConnection con = QDBusConnection::sessionBus();
        if (Q_UNLIKELY(!con.registerService(QStringLiteral("de.huessenbergnetz.fuoten")))) {
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
    view->rootContext()->setContextProperty(QStringLiteral("wipemanager"), wipeManager);
    view->rootContext()->setContextProperty(QStringLiteral("covercon"), new CoverConnector(app.get()));
    view->rootContext()->setContextProperty(QStringLiteral("_fuotenDbusProxy"), dbusproxy);

    view->setSource(SailfishApp::pathToMainQml());

    view->showFullScreen();

    return app->exec();
}

