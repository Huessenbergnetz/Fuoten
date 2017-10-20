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

#include <QtQml>
#include <QGuiApplication>
#include <QQuickView>
#include <QLocale>
#include <QTranslator>
#include <QDir>
#include <QStandardPaths>
#include <QNetworkDiskCache>

#ifndef CLAZY
#include <sailfishapp.h>
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

#ifndef CLAZY
#include "fuoteniconprovider.h"
#endif
#include "sharing/sharingmethodsmodel.h"
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
        txt = QStringLiteral("[%1] %2: %3").arg(t, QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss:zzz")), msg);
    }

    fprintf(stderr, "%s\n", txt.toLocal8Bit().constData());

    QFile logFile(QDir::homePath().append(QStringLiteral("/fuoten.log")));
    logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&logFile);
    ts << txt << endl;
#else
    Q_UNUSED(context)
    fprintf(stderr, "[%c] %s: %s\n", t.toLatin1(), QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss:zzz")).toLocal8Bit().constData(), msg.toLocal8Bit().constData());
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
        const QString l10nDir = SailfishApp::pathTo(QStringLiteral("l10n")).toString(QUrl::RemoveScheme);
#else
        const QString l10nDir;
#endif
        const QLocale locale;
        for (const QString &name : {QStringLiteral("fuoten"), QStringLiteral("libfuoten"), QStringLiteral("btsc")}) {
            auto trans = new QTranslator(app.data());
            if (Q_LIKELY(trans->load(locale, name, QStringLiteral("_"), l10nDir, QStringLiteral(".qm")))) {
                app->installTranslator(trans);
            }
        }

        QTranslator *tfeTrans = new QTranslator(app.data());
        if (locale.language() == QLocale::C) {

            if (Q_LIKELY(tfeTrans->load(QStringLiteral("sailfish_transferengine_plugins_eng_en"), QStringLiteral("/usr/share/translations")))) {
                app->installTranslator(tfeTrans);
            }

        } else {

            if (Q_LIKELY(tfeTrans->load(locale, QStringLiteral("sailfish_transferengine_plugins"), QStringLiteral("-"), QStringLiteral("/usr/share/translations"), QStringLiteral(".qm")))) {
                app->installTranslator(tfeTrans);
            }

        }
    }

    auto notificator = new SfosNotificator(config, app.data());
    QNetworkDiskCache *qmlDiskCache = nullptr;
    QThread storageThread;
    QObject::connect(app.data(), &QCoreApplication::aboutToQuit, [&storageThread]() {storageThread.quit(); storageThread.wait();});
    Fuoten::SQLiteStorage *sqliteStorage = nullptr;
    {
        QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
        QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
        QDir qmlCacheDir(cacheDir.absoluteFilePath(QStringLiteral("qmlcache")));

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
        sqliteStorage->moveToThread(&storageThread);
        storageThread.start();
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
    auto view = SailfishApp::createView();
    view->engine()->addImageProvider(QStringLiteral("fuoten"), new FuotenIconProvider);
    view->engine()->setNetworkAccessManagerFactory(namFactory.data());
#else
    QScopedPointer<QQuickView> view(new QQuickView);
#endif

    view->rootContext()->setContextProperty(QStringLiteral("config"), config);
    view->rootContext()->setContextProperty(QStringLiteral("localstorage"), sqliteStorage);
    view->rootContext()->setContextProperty(QStringLiteral("synchronizer"), synchronizer);
    view->rootContext()->setContextProperty(QStringLiteral("covercon"), new CoverConnector(app.data()));
    view->rootContext()->setContextProperty(QStringLiteral("cccmmm"), QString::fromUtf8(QByteArray::fromBase64(QByteArrayLiteral("a29udGFrdEBodWVzc2VuYmVyZ25ldHouZGU="))));

#ifndef CLAZY
    view->setSource(SailfishApp::pathTo(QStringLiteral("qml/harbour-fuoten.qml")));
#endif

    view->show();

    return app->exec();
}

