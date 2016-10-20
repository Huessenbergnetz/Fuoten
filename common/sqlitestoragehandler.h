/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * common/sqlitestoragehandler.h
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

#ifndef SQLITESTORAGEHANDLER_H
#define SQLITESTORAGEHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#include <Helpers/storagehandler.h>
#include <error.h>

class QJsonDocument;

class SQLiteStorageHandler : public Fuoten::StorageHandler
{
    Q_OBJECT
    /*!
     * \brief Returns true when the databases have been setup successfully.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>ready() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>readyChanged(bool ready)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)
    /*!
     * \brief Pointer to a Fuoten::Error object, if any error occures.
     *
     * \par Access functions:
     * <TABLE><TR><TD>FuotenError*</TD><TD>error() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>errorChanged(FuotenError *error)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::Error *error READ error NOTIFY errorChanged)
public:
    /*!
     * \brief Constructs a new SQLiteStorageHandler object.
     */
    explicit SQLiteStorageHandler(QObject *parent = nullptr);

    bool ready() const;
    Fuoten::Error *error() const;

public slots:
    void foldersRequested(const QJsonDocument &json) Q_DECL_OVERRIDE;
    void folderCreated(const QJsonDocument &json) Q_DECL_OVERRIDE;

    void databaseReady();

signals:
    void readyChanged(bool ready);
    void errorChanged(Fuoten::Error *error);

private:
    Q_DISABLE_COPY(SQLiteStorageHandler)

    bool m_ready;
    Fuoten::Error *m_error;
    QSqlDatabase m_db;

    void setError(Fuoten::Error *nError);
};

#endif // SQLITESTORAGEHANDLER_H
