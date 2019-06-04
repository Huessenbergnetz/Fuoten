/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2019 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
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

#include "licensesmodel.h"
#include <Fuoten/fuoten.h>

LicensesModel::LicensesModel(QObject *parent) : Hbnsc::LicenseModel(parent)
{
    add(QStringLiteral("Libfuoten"),
        QStringLiteral("Matthias Fehring"),
        Fuoten::version().toString(),
        QUrl(QStringLiteral("https://github.com/Huessenbergnetz/libfuoten")),
        //: description for libfuoten in the list of used 3rd party components
        //% "Libfuoten is a Qt based library that provides access the ownCloud/Nextcloud News App API."
        qtTrId("fuoten-libfuoten-desc"),
        QStringLiteral("GNU Lesser General Public License, Version 3"),
        QStringLiteral("LGPLv3.qml"),
        QUrl(QStringLiteral("https://github.com/Huessenbergnetz/libfuoten/blob/master/LICENSE")),
        QUrl());

    add(QStringLiteral("Libfuoten Translations"),
        QStringLiteral("Libfuoten Translators"),
        QString(),
        QUrl(QStringLiteral("https://www.transifex.com/huessenbergnetz/libfuoten/")),
        //: description for libintfuorit translations in the list of used 3rd party components
        //% "Libfuoten is a Qt based library that provides access the ownCloud/Nextcloud News App API. Translations are provided by the community."
        qtTrId("fuoten-libfuoten-trans-desc"),
        QStringLiteral("Creative Commons Attribution 4.0 International Public License"),
        QStringLiteral("CC-BY-4_0.qml"),
        QUrl(QStringLiteral("https://github.com/Huessenbergnetz/libfuoten/blob/master/LICENSE.translations")),
        QUrl());

    add(QStringLiteral("Fuoten Translations"),
        QStringLiteral("Fuoten Translators"),
        QString(),
        QUrl(QStringLiteral("https://www.transifex.com/huessenbergnetz/fuoten")),
        //: description for Fuoten translations in the list of used 3rd party components
        //% "The translations for Fuoten are provided by the community. To see who is responsible for which translation, open the contributors page."
        qtTrId("fuoten-trans-desc"),
        QStringLiteral("Creative Commons Attribution 4.0 International Public License"),
        QStringLiteral("CC-BY-4_0.qml"),
        QUrl(QStringLiteral("https://github.com/Huessenbergnetz/Fuoten/blob/master/LICENSE.translations")),
        QUrl());

    add(QStringLiteral("Rescaling Richtext QML"),
        QStringLiteral("Martin Grimme"),
        QString(),
        QUrl(QStringLiteral("https://github.com/pycage/tidings")),
        //: description for Rescaling Richtext QML in the list of used 3rd party components
        //% "Rescaling Richtext QML part was originally created by Martin Grimme for the Tidings RSS feed reader and is used to rescale richtext views with images included."
        qtTrId("fuoten-rescaling-richtext-qml-desc"),
        QStringLiteral("GNU General Public License, Version 2"),
        QStringLiteral("GPLv2.qml"),
        QUrl(QStringLiteral("https://github.com/pycage/tidings/blob/master/LICENSE")),
        QUrl());

    add(QStringLiteral("ADVobfuscator"),
        QStringLiteral("Sebastien Andrivet"),
        QString(),
        QUrl(QStringLiteral("https://github.com/andrivet/ADVobfuscator")),
        //: description for ADVobfuscator in the list of used 3rd party components
        //% "ADVobfuscator is an obfuscation library based on C++11/14 and metaprogramming. It is used by Fuoten to obfuscate the internal used AES key to encrypt stored passwords."
        qtTrId("fuoten-advobfuscator-desc"),
        QStringLiteral("Modified BSD License"),
        QStringLiteral("BSD-3.qml"),
        QUrl(),
        QUrl());

    addNemoNotifications();
    addOpenSSL();
    addSQLite();

    sortLicenses();
}

LicensesModel::~LicensesModel()
{

}
