/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SFOSMIGRATOR_H
#define SFOSMIGRATOR_H

#include <QString>

class SfosMigrator
{
public:
    /*!
     * \brief Returns the path to the data directory.
     */
    static QString dataDirPath();
    /*!
     * \brief Returns the absolute path to data file.
     */
    static QString dataFilename();
    /*!
     * \brief Returns the path to the config directory.
     */
    static QString configDirPath();
    /*!
     * \brief Returns the absolute path to the config file.
     */
    static QString configFilename();

    static bool migrateData();
    static bool migrateConfig();
    static bool migrate();
private:
    SfosMigrator();
    ~SfosMigrator();
};

#endif // SFOSMIGRATOR_H
