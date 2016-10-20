/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * common/sqlitestoragehandler.cpp
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

#include "sqlitestoragehandler.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

SQLiteStorageHandler::SQLiteStorageHandler(QObject *parent) : Fuoten::StorageHandler(parent)
{
    m_ready = false;
    m_error = nullptr;
}



void SQLiteStorageHandler::foldersRequested(const QJsonDocument &json)
{

}



void SQLiteStorageHandler::folderCreated(const QJsonDocument &json)
{

}



void SQLiteStorageHandler::databaseReady()
{
    if (!m_ready) {


        if (!QSqlDatabase::connectionNames().contains(QStringLiteral("fuotendb"))) {

            QString dbpath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
            if (dbpath.endsWith(QChar('/'))) {
                dbpath.append(QStringLiteral("database.sqlite"));
            } else {
                dbpath.append(QStringLiteral("/database.sqlite"));
            }

            m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QLatin1String("fuotendb"));
            m_db.setDatabaseName(dbpath);
            m_db.open();

        } else {
            m_db = QSqlDatabase::database(QStringLiteral("fuotendb"));
        }

        if (!m_db.isOpen()) {
            qFatal("%s: %s", "Failed to open database", m_db.lastError().text().toLocal8Bit().constData());
            return;
        }
#ifdef QT_DEBUG
        else
        {
            qDebug() << "Successfully opened database.";
        }
#endif

        m_ready = true;
        emit readyChanged(true);
    }
}



bool SQLiteStorageHandler::ready() const { return m_ready; }

Fuoten::Error *SQLiteStorageHandler::error() const { return m_error; }

void SQLiteStorageHandler::setError(Fuoten::Error *nError)
{
    if (nError != m_error) {
        Fuoten::Error *old = m_error;
        m_error = nError;
#ifdef QT_DEBUG
        qDebug() << "Changed error to" << m_error;
#endif
        emit errorChanged(m_error);

        if (old) {
            if (old->parent() == this) {
                delete old;
            }
        }
    }
}



