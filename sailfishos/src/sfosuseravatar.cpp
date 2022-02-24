/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "sfosuseravatar.h"

SfosUserAvatar::SfosUserAvatar(QObject *parent) : UserAvatar(parent)
{

}

SfosUserAvatar::~SfosUserAvatar() = default;

QUrl SfosUserAvatar::defaultAvatarUrl() const
{
    return QUrl(QStringLiteral("image://theme/icon-l-people"));
}

#include "moc_sfosuseravatar.cpp"
