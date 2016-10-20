/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * common/sqlitemanager.h
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

#ifndef SQLITEMANAGER_H
#define SQLITEMANAGER_H

#include <QObject>
#include <QThread>
#include <QSqlDatabase>

class QSqlError;

class SQLiteManager : public QThread
{
    Q_OBJECT
public:
    explicit SQLiteManager(QObject *parent = 0);

protected:
    void run() Q_DECL_OVERRIDE;

signals:
    void databaseReady();

private:
    Q_DISABLE_COPY(SQLiteManager)

    bool createDatabase();
    void fatalError(const char *message, const QSqlError &error);

    bool m_ready;
    quint16 m_currentDbVersion;
    QSqlDatabase m_db;
};

#endif // SQLITEMANAGER_H
