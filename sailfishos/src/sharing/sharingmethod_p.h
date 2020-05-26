/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2019 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/src/sharing/sharingmethod_p.h
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

#ifndef SHARINGMETHOD_P_H
#define SHARINGMETHOD_P_H

#include "sharingmethod.h"
#include <QSharedData>

class SharingMethodData : public QSharedData
{
public:
    SharingMethodData() {}

    SharingMethodData(const SharingMethodData &other) :
        QSharedData(other),
        displayName(other.displayName),
        userName(other.userName),
        methodId(other.methodId),
        shareUiPath(other.shareUiPath),
        iconUrl(other.iconUrl),
        capabilities(other.capabilities),
        args(other.args),
        accountId(other.accountId)
    {}

    QString displayName;
    QString userName;
    QString methodId;
    QString shareUiPath;
    QUrl iconUrl;
    QStringList capabilities;
    QVariantMap args;
    quint32 accountId;
};

#endif // SHARINGMETHOD_P_H
