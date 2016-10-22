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
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QHash>
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
    if (!m_ready) {
        return;
    }

    if (json.isEmpty() || json.isNull()) {
        return;
    }

    const QJsonArray folders = json.object().value(QStringLiteral("folders")).toArray();

#ifdef QT_DEBUG
    qDebug() << "Processing" << folders.size() << "folders requested from the remote server.";
#endif

    QHash<quint64, QString> reqFolders;

    for (const QJsonValue &f : folders) {
        QJsonObject o = f.toObject();
        if (!o.isEmpty()) {
            reqFolders.insert(o.value(QStringLiteral("id")).toVariant().toULongLong(), o.value(QStringLiteral("name")).toString());
        }
    }

    QSqlQuery q(m_db);

    // query the currently local available folders in the database
    QHash<quint64, QString> currentFolders;

    if (!q.exec(QStringLiteral("SELECT id, name FROM folders"))) {
        //% "Failed to query the folders from the local database."
        setQueryError(qtTrId("fuoten-failed-query-folders"), &q);
        return;
    }

    while (q.next()) {
        currentFolders.insert(q.value(0).toULongLong(), q.value(1).toString());
    }

    if (reqFolders.isEmpty() && currentFolders.isEmpty()) {
#ifdef QT_DEBUG
        qDebug() << "Nothing to do. Returning.";
#endif
        return;
    }

    QList<quint64> deletedIds;
    QList<QPair<quint64, QString>> newFolders;
    QList<QPair<quint64, QString>> updatedFolders;

    if (currentFolders.isEmpty()) {

        // add all requested folders to the list of new folders
        QHash<quint64, QString>::const_iterator i = reqFolders.constBegin();
        while (i != reqFolders.constEnd()) {
            newFolders.append(QPair<quint64, QString>(i.key(), i.value()));
            ++i;
        }

    } else {

        // checking for updated and deleted folders
        QHash<quint64, QString>::const_iterator i = currentFolders.constBegin();
        while (i != currentFolders.constEnd()) {
            if (reqFolders.contains(i.key())) {
                if (reqFolders.value(i.key()) != i.value()) {
                    updatedFolders.append(qMakePair(i.key(), reqFolders.value(i.key())));
                }
            } else {
                deletedIds << i.key();
            }
            ++i;
        }

        // checking for newly added folders
        i = reqFolders.constBegin();
        while (i != reqFolders.constEnd()) {
            if (!currentFolders.contains(i.key())) {
                newFolders.append(qMakePair(i.key(), i.value()));
            }
            ++i;
        }
    }

    // start updating the database
    if (!deletedIds.isDetached() || !newFolders.isEmpty() || !updatedFolders.isEmpty()) {

        if (!m_db.transaction()) {
            //% "Failed to begin a database transaction."
            setDbError(qtTrId("fuoten-error-transaction-begin"));
            return;
        }

        if (!deletedIds.isEmpty()) {

            for (int i = 0; i < deletedIds.size(); ++i) {

#ifdef QT_DEBUG
                qDebug() << "Deleting folder with ID" << deletedIds.at(i) << "from local database.";
#endif

                if (!q.prepare(QStringLiteral("DELETE FROM folders WHERE id = ?"))) {
                    //% "Failed to prepare database query."
                    setQueryError(qtTrId("fuoten-error-failed-prepare-query"), &q);
                    return;
                }

                q.addBindValue(deletedIds.at(i));

                if (!q.exec()) {
                    //% "Failed to execute database query."
                    setQueryError(qtTrId("fuoten-error-failed-execute-query"), &q);
                    return;
                }
            }
        }

        if (!updatedFolders.isEmpty()) {

            for (int i = 0; i < updatedFolders.size(); ++i) {

#ifdef QT_DEBUG
                qDebug() << "Updating name of folder with ID" << updatedFolders.at(i).first << "in local database to:" << updatedFolders.at(i).second;
#endif

                if (!q.prepare(QStringLiteral("UPDATE folders SET name = ? WHERE id = ?"))) {
                    //% "Failed to prepare database query."
                    setQueryError(qtTrId("fuoten-error-failed-prepare-query"), &q);
                    return;
                }

                q.addBindValue(updatedFolders.at(i).second);
                q.addBindValue(updatedFolders.at(i).first);

                if (!q.exec()) {
                    //% "Failed to execute database query."
                    setQueryError(qtTrId("fuoten-error-failed-execute-query"), &q);
                    return;
                }
            }

        }


        if (!newFolders.isEmpty()) {

            for (int i = 0; i < newFolders.size(); ++i) {

#ifdef QT_DEBUG
                qDebug() << "Adding folder" << newFolders.at(i).second << "with ID" << newFolders.at(i).first << "to the local database.";
#endif
                if (!q.prepare(QStringLiteral("INSERT INTO folders (id, name) VALUES (?, ?)"))) {
                    //% "Failed to prepare database query."
                    setQueryError(qtTrId("fuoten-error-failed-prepare-query"), &q);
                    return;
                }

                q.addBindValue(newFolders.at(i).first);
                q.addBindValue(newFolders.at(i).second);

                if (!q.exec()) {
                    //% "Failed to execute database query."
                    setQueryError(qtTrId("fuoten-error-failed-execute-query"), &q);
                    return;
                }
            }
        }



        if (!m_db.commit()) {
            //% "Failed to commit a database transaction."
            setDbError(qtTrId("fuoten-error-transaction-commit"));
            return;
        }

    }

    emit requestedFolders(updatedFolders, newFolders, deletedIds);
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



void SQLiteStorageHandler::setDbError(const QString &message)
{
    setError(new Fuoten::Error(Fuoten::Error::ApplicationError, Fuoten::Error::Critical, message, m_db.lastError().databaseText(), this));
}


void SQLiteStorageHandler::setQueryError(const QString &message, QSqlQuery *q)
{
    if (q) {
        setError(new Fuoten::Error(Fuoten::Error::ApplicationError, Fuoten::Error::Critical, message, q->lastError().text(), this));
    }
}
