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



