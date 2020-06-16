/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
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
