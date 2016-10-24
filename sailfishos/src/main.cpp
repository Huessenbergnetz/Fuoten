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

#include <error.h>
#include <fuoten.h>
#include <Helpers/accountvalidator.h>
#include <Helpers/configuration.h>
#include <Helpers/synchronizer.h>
#include <Storage/sqlitestorage.h>
#include <Storage/abstractstorage.h>
#include <Models/folderlistmodel.h>
#include <Models/folderlistfiltermodel.h>
#include <API/renamefolder.h>

#include "../../common/configuration.h"
#include "../../common/languagemodel.h"

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
    QGuiApplication* app = SailfishApp::application(argc, argv);
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

    Configuration config;

    if (!config.language().isEmpty()) {
        QLocale::setDefault(QLocale(config.language()));
    } else {
        QLocale::setDefault(QLocale::system());
    }

#ifndef CLAZY
    const QString l10nDir = SailfishApp::pathTo(QStringLiteral("l10n")).toString(QUrl::RemoveScheme);
    QTranslator *appTrans = new QTranslator(app);
    if (appTrans->load(QLocale(), QStringLiteral("fuoten"), QStringLiteral("_"), l10nDir, QStringLiteral(".qm"))) {
        app->installTranslator(appTrans);
    }

    QTranslator *libTrans = new QTranslator(app);
    if (libTrans->load(QLocale(), QStringLiteral("libfuoten"), QStringLiteral("_"), l10nDir, QStringLiteral(".qm"))) {
        app->installTranslator(libTrans);
    }

    QTranslator *btscTrans = new QTranslator(app);
    if (btscTrans->load(QLocale(), QStringLiteral("btsc"), QStringLiteral("_"), l10nDir, QStringLiteral(".qm"))) {
        app->installTranslator(btscTrans);
    }
#endif

    QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

    if (!dataDir.exists()) {
        if (!dataDir.mkpath(dataDir.absolutePath()))
        qFatal("Failed to create data directory.");
    }

    Fuoten::SQLiteStorage sqliteStorage(dataDir.absoluteFilePath(QStringLiteral("database.sqlite")));
    sqliteStorage.init();

    Fuoten::Synchronizer synchronizer;
    synchronizer.setConfiguration(&config);
    synchronizer.setStorage(&sqliteStorage);

    qmlRegisterUncreatableType<Fuoten::FuotenEnums>("harbour.fuoten", 1, 0, "Fuoten", QStringLiteral("You can not create a Fuoten object"));
    qmlRegisterUncreatableType<Fuoten::Configuration>("harbour.fuoten", 1, 0, "FuotenConfiguration", QStringLiteral("You can not create a FuotenConfiguration object."));
    qmlRegisterType<Fuoten::Error>("harbour.fuoten", 1, 0, "FuotenError");
    qmlRegisterType<Fuoten::AccountValidator>("harbour.fuoten", 1, 0, "AccountValidator");
    qmlRegisterUncreatableType<Fuoten::AbstractStorage>("harbour.fuoten", 1, 0, "AbstractStorage", QStringLiteral("You can not create an AbstractStorage object."));

    qmlRegisterType<Fuoten::FolderListModel>("harbour.fuoten.models", 1, 0, "FolderListModel");
    qmlRegisterType<Fuoten::FolderListFilterModel>("harbour.fuoten.models", 1, 0, "FolderListFilterModel");

    qmlRegisterType<Fuoten::RenameFolder>("harbour.fuoten.api", 1, 0, "RenameFolder");

    qmlRegisterType<LanguageModel>("harbour.fuoten", 1, 0, "LanguageModel");
    qmlRegisterUncreatableType<Configuration>("harbour.fuoten", 1, 0, "Configuratoin", QStringLiteral("You can not create a Configuration object"));

#ifndef CLAZY
    QQuickView *view = SailfishApp::createView();
#else
    QQuickView *view = new QQuickView();
#endif

    view->rootContext()->setContextProperty(QStringLiteral("config"), &config);
    view->rootContext()->setContextProperty(QStringLiteral("localstorage"), &sqliteStorage);
    view->rootContext()->setContextProperty(QStringLiteral("synchronizer"), &synchronizer);

#ifndef CLAZY
    view->setSource(SailfishApp::pathTo(QStringLiteral("qml/harbour-fuoten.qml")));
#endif

    view->show();

    return app->exec();
}

