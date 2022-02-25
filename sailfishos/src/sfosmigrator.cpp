#include "sfosmigrator.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QCoreApplication>

SfosMigrator::SfosMigrator()
{

}

SfosMigrator::~SfosMigrator()
{

}

QString SfosMigrator::dataDirPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + QCoreApplication::organizationName() + QLatin1Char('/') + QCoreApplication::applicationName();
}

QString SfosMigrator::dataFilename()
{
     return SfosMigrator::dataDirPath() + QStringLiteral("/database.sqlite");
}

QString SfosMigrator::configDirPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + QLatin1Char('/') + QCoreApplication::organizationName() + QLatin1Char('/') + QCoreApplication::applicationName();
}

QString SfosMigrator::configFilename()
{
    return SfosMigrator::configDirPath() + QLatin1Char('/') + QCoreApplication::applicationName() + QStringLiteral(".conf");
}

bool SfosMigrator::migrateData()
{
    QFileInfo newData(SfosMigrator::dataFilename());

    if (Q_LIKELY(newData.exists())) {
        qDebug("Data is already migrated to %s", qUtf8Printable(newData.absoluteFilePath()));
        return true;
    }

    QFile oldData(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + QCoreApplication::applicationName() + QLatin1Char('/') + QCoreApplication::applicationName() + QStringLiteral("/database.sqlite"));

    if (!oldData.exists()) {
        qDebug("No data to migrate, old data does not exist at %s", qUtf8Printable(oldData.fileName()));
        return true;
    }

    QDir newDataDir(SfosMigrator::dataDirPath());
    if (Q_UNLIKELY(!newDataDir.mkpath(SfosMigrator::dataDirPath()))) {
        qCritical("Failed to create new config directory %s", qUtf8Printable(SfosMigrator::dataDirPath()));
        return false;
    }

    if (Q_LIKELY(oldData.copy(newData.absoluteFilePath()))) {
        qInfo("Successfully migrated data from %s to %s", qUtf8Printable(oldData.fileName()), qUtf8Printable(newData.absoluteFilePath()));
    } else {
        qCritical("Failed to copy %s to %s: %s", qUtf8Printable(oldData.fileName()), qUtf8Printable(newData.absoluteFilePath()), qUtf8Printable(oldData.errorString()));
        return false;
    }

    return true;
}

bool SfosMigrator::migrateConfig()
{
    QFileInfo newSettings(SfosMigrator::configFilename());

    if (Q_LIKELY(newSettings.exists())) {
        qDebug("Settings are already migrated to %s", qUtf8Printable(newSettings.absoluteFilePath()));
        return true;
    }

    QFile oldSettings(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + QLatin1Char('/') + QCoreApplication::applicationName() + QLatin1Char('/') + QCoreApplication::applicationName() + QStringLiteral(".conf"));

    if (!oldSettings.exists()) {
        qDebug("No settings to migrate, old settings do not exist at %s", qUtf8Printable(oldSettings.fileName()));
        return true;
    }

    QDir newConfigDir(SfosMigrator::configDirPath());
    if (Q_UNLIKELY(!newConfigDir.mkpath(SfosMigrator::configDirPath()))) {
        qCritical("Failed to create new config directory %s", qUtf8Printable(SfosMigrator::configDirPath()));
        return false;
    }

    if (Q_LIKELY(oldSettings.copy(newSettings.absoluteFilePath()))) {
        qInfo("Successfully migrated settings from %s to %s", qUtf8Printable(oldSettings.fileName()), qUtf8Printable(newSettings.absoluteFilePath()));
    } else {
        qCritical("Failed to copy %s to %s: %s", qUtf8Printable(oldSettings.fileName()), qUtf8Printable(newSettings.absoluteFilePath()), qUtf8Printable(oldSettings.errorString()));
        return false;
    }

    return true;
}

bool SfosMigrator::migrate()
{
    if (SfosMigrator::migrateData()) {
        return SfosMigrator::migrateConfig();
    } else {
        return false;
    }
}
