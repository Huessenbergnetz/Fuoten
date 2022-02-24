/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SHARINGMETHOD_P_H
#define SHARINGMETHOD_P_H

#include "sharingmethod.h"
#include <QSharedData>

class SharingMethodData : public QSharedData
{
public:
//    SharingMethodData() : QSharedData() {}

//    SharingMethodData(const SharingMethodData &other) :
//        QSharedData(other),
//        displayName(other.displayName),
//        userName(other.userName),
//        methodId(other.methodId),
//        shareUiPath(other.shareUiPath),
//        iconUrl(other.iconUrl),
//        capabilities(other.capabilities),
//        args(other.args),
//        accountId(other.accountId)
//    {}

    QString displayName;
    QString userName;
    QString methodId;
    QString shareUiPath;
    QUrl iconUrl;
    QStringList capabilities;
    QVariantMap args;
    quint32 accountId = 0;
};

#endif // SHARINGMETHOD_P_H
