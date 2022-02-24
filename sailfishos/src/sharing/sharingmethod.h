/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SHARINGMETHOD_H
#define SHARINGMETHOD_H

#include <QSharedDataPointer>
#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <QUrl>
#include <QMetaType>
#include <QDBusArgument>

class SharingMethodData;

class SharingMethod
{
public:
    SharingMethod();

    SharingMethod(const SharingMethod &other);

    SharingMethod(SharingMethod &&other) noexcept;

    SharingMethod& operator=(const SharingMethod &other);

    SharingMethod& operator=(SharingMethod &&other) noexcept;

    ~SharingMethod();

    QString displayName() const;
    void setDisplayName(const QString &nDisplayName);

    QString userName() const;
    void setUserName(const QString &nUserName);

    QString methodId() const;
    void setMethodId(const QString &nMethodId);

    QString shareUiPath() const;
    void setShareUiPath(const QString &nShareUiPath);

    QStringList capabilites() const;
    void setCapabilities(const QStringList &nCapabilities);

    quint32 accountId() const;
    void setAccountId(quint32 nAccountId);

    QUrl iconUrl() const;
    void setIconUrl(const QUrl &nIconUrl);

    QVariantMap args() const;
    void setArgs(const QVariantMap &nArgs);

protected:
    QSharedDataPointer<SharingMethodData> d;
};

Q_DECLARE_METATYPE(SharingMethod)
Q_DECLARE_TYPEINFO(SharingMethod, Q_MOVABLE_TYPE);

QDBusArgument& operator<<(QDBusArgument &argument, const SharingMethod &sm);
const QDBusArgument& operator>>(const QDBusArgument &argument, SharingMethod &sm);

QDebug operator<<(QDebug dbg, const SharingMethod &sm);

#endif // SHARINGMETHOD_H
