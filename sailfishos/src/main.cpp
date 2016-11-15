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
#include <Fuoten/API/CreateFolder>
#include <Fuoten/Folder>
#include <Fuoten/Feed>
#include <Fuoten/Article>
#include <Fuoten/API/CreateFeed>

#include <Fuoten/Models/FeedListModel>
#include <Fuoten/Models/FeedListFilterModel>
#include <Fuoten/Models/ArticleListModel>
#include <Fuoten/Models/ArticleListFilterModel>

#include "../../common/configuration.h"
#include "../../common/languagemodel.h"
#include "../../common/enums.h"
#include "../../common/contextconfig.h"
#include "../../common/imagecache.h"

#ifdef QT_DEBUG
void fuotenMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString t;
    switch (type) {
    case QtDebugMsg:
        t = QStringLiteral("D");
        break;
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    case QtInfoMsg:
        t = QStringLiteral("I");
        break;
#endif
    case QtWarningMsg:
        t = QStringLiteral("W");
        break;
    case QtCriticalMsg:
        t = QStringLiteral("C");
        break;
    case QtFatalMsg:
        t = QStringLiteral("F");
        break;
    }

    QRegularExpression re(QStringLiteral("([\\w:]+)\\("));

    QString txt;

    if (context.function) {

        txt = QStringLiteral("[%1] %2: %3:%4 - %5").arg(t,
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

    if (type == QtFatalMsg) {
        abort();
    }
}
#endif



int main(int argc, char *argv[])
{
#ifndef CLAZY
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
#else
    QGuiApplication* app = new QGuiApplication(argc, argv);
#endif

    app->setApplicationName(QStringLiteral("harbour-fuoten"));
    app->setApplicationDisplayName(QStringLiteral("Fuoten"));
    app->setApplicationVersion(QStringLiteral(VERSION_STRING));

#ifdef QT_DEBUG
    QFile::remove(QDir::homePath().append(QStringLiteral("/fuoten.log")));
    qInstallMessageHandler(fuotenMessageHandler);
#endif

    QScopedPointer<Configuration> config(new Configuration);

    if (!config->language().isEmpty()) {
        QLocale::setDefault(QLocale(config->language()));
    } else {
        QLocale::setDefault(QLocale::system());
    }

#ifndef CLAZY
    const QString l10nDir = SailfishApp::pathTo(QStringLiteral("l10n")).toString(QUrl::RemoveScheme);
    QTranslator *appTrans = new QTranslator(app.data());
    if (appTrans->load(QLocale(), QStringLiteral("fuoten"), QStringLiteral("_"), l10nDir, QStringLiteral(".qm"))) {
        app->installTranslator(appTrans);
    }

    QTranslator *libTrans = new QTranslator(app.data());
    if (libTrans->load(QLocale(), QStringLiteral("libfuoten"), QStringLiteral("_"), l10nDir, QStringLiteral(".qm"))) {
        app->installTranslator(libTrans);
    }

    QTranslator *btscTrans = new QTranslator(app.data());
    if (btscTrans->load(QLocale(), QStringLiteral("btsc"), QStringLiteral("_"), l10nDir, QStringLiteral(".qm"))) {
        app->installTranslator(btscTrans);
    }
#endif

    QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));

    if (!dataDir.exists()) {
        if (!dataDir.mkpath(dataDir.absolutePath())) {
            qFatal("Failed to create data directory.");
        }
    }

    if (!cacheDir.exists()) {
        if (!cacheDir.mkpath(cacheDir.absolutePath())) {
            qFatal("Failed to create cache directory.");
        }
    }

    QScopedPointer<Fuoten::SQLiteStorage> sqliteStorage(new Fuoten::SQLiteStorage(dataDir.absoluteFilePath(QStringLiteral("database.sqlite"))));
    sqliteStorage->init();

    QScopedPointer<Fuoten::Synchronizer> synchronizer(new Fuoten::Synchronizer);
    synchronizer->setConfiguration(config.data());
    synchronizer->setStorage(sqliteStorage.data());

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

    qmlRegisterType<Fuoten::Folder>("harbour.fuoten.items", 1, 0, "Folder");
    qmlRegisterType<Fuoten::Feed>("harbour.fuoten.items", 1, 0, "Feed");
    qmlRegisterType<Fuoten::Article>("harbour.fuoten.items", 1, 0, "Article");

    qmlRegisterType<LanguageModel>("harbour.fuoten", 1, 0, "LanguageModel");
    qmlRegisterUncreatableType<Configuration>("harbour.fuoten", 1, 0, "Configuration", QStringLiteral("You can not create a Configuration object"));

    qmlRegisterUncreatableType<FuotenAppEnums>("harbour.fuoten", 1, 0, "FuotenApp", QStringLiteral("You can not create a FuotenApp object."));
    qmlRegisterType<ContextConfig>("harbour.fuoten", 1, 0, "ContextConfig");
    qmlRegisterType<ImageCache>("harbour.fuoten", 1, 0, "ImageCache");

#ifndef CLAZY
    QScopedPointer<QQuickView> view(SailfishApp::createView());
#else
    QQuickView *view = new QQuickView();
#endif

    view->rootContext()->setContextProperty(QStringLiteral("config"), config.data());
    view->rootContext()->setContextProperty(QStringLiteral("localstorage"), sqliteStorage.data());
    view->rootContext()->setContextProperty(QStringLiteral("synchronizer"), synchronizer.data());

#ifndef CLAZY
    view->setSource(SailfishApp::pathTo(QStringLiteral("qml/harbour-fuoten.qml")));
#endif

    view->show();

    return app->exec();
}

