/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * common/sqlitemanager.cpp
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

#include "sqlitemanager.h"
#include <QVariant>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QDir>
#ifdef QT_DEBUG
#include <QDebug>
#endif

SQLiteManager::SQLiteManager(QObject *parent) : QThread(parent)
{
    m_ready = false;
    m_currentDbVersion = 0;
}



void SQLiteManager::run()
{
    QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

    if (!dataDir.exists()) {
        if (!dataDir.mkpath(dataDir.absolutePath()))
        qFatal("Failed to create data directory.");
        return;
    }

    m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QLatin1String("fuotendb"));
    m_db.setDatabaseName(dataDir.absoluteFilePath(QStringLiteral("database.sqlite")));

    if (!m_db.open()) {
        fatalError("Failed to open database", m_db.lastError());
    }

    if (!createDatabase()) {
        return;
    }

    emit databaseReady();
}



bool SQLiteManager::createDatabase()
{
#ifdef QT_DEBUG
    qDebug() << "Start checking database scheme";
#endif

    QSqlQuery q(m_db);

    if (!q.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
        fatalError("Failed to activate foreign keys support", q.lastError());
        return false;
    }

    if (!m_db.transaction()) {
        fatalError("Failed to create database transaction", q.lastError());
        return false;
    }

    if (!q.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS system "
                               "(id INTEGER PRIMARY KEY NOT NULL, "
                               "key TEXT NOT NULL, "
                               "value TEXT NO NULL)"
                               ))) {
        fatalError("Failed to create system table", q.lastError());
        return false;
    }

    if (!q.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS folders "
                               "(id INTEGER PRIMARY KEY NOT NULL, "
                               "parent_id INTEGER, "
                               "name VARCHAR NOT NULL, "
                               "localUnreadCount INTEGER DEFAULT 0)"
                               ))) {
        fatalError("Failed to create folders table", q.lastError());
        return false;
    }

    if (!m_db.commit()) {
        fatalError("Failed to commit database transaction", q.lastError());
        return false;
    }

    if (q.exec(QStringLiteral("SELECT value FROM system WHERE key = 'schema_version'"))) {
        if (q.next()) {
            m_currentDbVersion = q.value(0).toUInt();
        }
    } else {
        fatalError("Failed to query current database scheme version", q.lastError());
        return false;
    }

    if (m_currentDbVersion == 0) {
        if (!q.exec(QStringLiteral("INSERT INTO system (key, value) VALUES ('schema_version', '1')"))) {
            fatalError("Failed to execute database query", q.lastError());
            return false;
        }
    }

#ifdef QT_DEBUG
    qDebug() << "Finished checking database scheme";
#endif

    return true;
}




void SQLiteManager::fatalError(const char *message, const QSqlError &error)
{
    qFatal("%s: %s", message, error.text().toLocal8Bit().constData());
}
