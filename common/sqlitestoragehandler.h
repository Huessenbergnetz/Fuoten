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
