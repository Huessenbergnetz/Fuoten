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
#endif

#include <QtQml>
#include <QGuiApplication>
#include <QQuickView>
#include <QTranslator>

#ifndef CLAZY
#include <sailfishapp.h>
#endif

#include "../../common/globals.h"


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

#ifndef CLAZY
    QQuickView *view = SailfishApp::createView();
#else
    QQuickView *view = new QQuickView();
#endif

#ifndef CLAZY
    view->setSource(SailfishApp::pathTo(QStringLiteral("qml/harbour-fuoten.qml")));
#endif

    view->show();

    return app->exec();
}

