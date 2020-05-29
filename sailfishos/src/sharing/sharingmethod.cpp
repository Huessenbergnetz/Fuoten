/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2019 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/src/sharing/sharingmethod.cpp
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

#include "sharingmethod_p.h"
#include <QDebug>

SharingMethod::SharingMethod() :
    d(new SharingMethodData)
{

}

SharingMethod::SharingMethod(const SharingMethod &other) = default;

SharingMethod::SharingMethod(SharingMethod &&other) noexcept = default;

SharingMethod& SharingMethod::operator=(const SharingMethod &other) = default;

SharingMethod& SharingMethod::operator=(SharingMethod &&other) noexcept = default;

SharingMethod::~SharingMethod() = default;

QString SharingMethod::displayName() const
{
    return d->displayName;
}


void SharingMethod::setDisplayName(const QString &nDisplayName)
{
    d->displayName = nDisplayName;
}


QString SharingMethod::userName() const
{
    return d->userName;
}


void SharingMethod::setUserName(const QString &nUserName)
{
    d->userName = nUserName;
}


QString SharingMethod::methodId() const
{
    return d->methodId;
}


void SharingMethod::setMethodId(const QString &nMethodId)
{
    d->methodId = nMethodId;
}


QString SharingMethod::shareUiPath() const
{
    return d->shareUiPath;
}


void SharingMethod::setShareUiPath(const QString &nShareUiPath)
{
    d->shareUiPath = nShareUiPath;
}

QStringList SharingMethod::capabilites() const
{
    return d->capabilities;
}


void SharingMethod::setCapabilities(const QStringList &nCapabilities)
{
    d->capabilities = nCapabilities;
}


quint32 SharingMethod::accountId() const
{
    return d->accountId;
}


void SharingMethod::setAccountId(quint32 nAccountId)
{
    d->accountId = nAccountId;
}


QUrl SharingMethod::iconUrl() const
{
    return d->iconUrl;
}


void SharingMethod::setIconUrl(const QUrl &nIconUrl)
{
    d->iconUrl = nIconUrl;
}


QVariantMap SharingMethod::args() const
{
    return d->args;
}


void SharingMethod::setArgs(const QVariantMap &nArgs)
{
    d->args = nArgs;
}


QDBusArgument &operator <<(QDBusArgument &argument, const SharingMethod &sm)
{
    argument.beginStructure();

    argument << sm.displayName()
             << sm.userName()
             << sm.methodId()
             << sm.shareUiPath()
             << sm.capabilites()
             << sm.accountId()
             << sm.iconUrl().toString()
             << sm.args();

    argument.endStructure();

    return argument;
}


const QDBusArgument &operator >>(const QDBusArgument &argument, SharingMethod &sm)
{
    QString displayName, userName, methodId, shareUiPath, iconUrl;
    QStringList capabilities;
    quint32 accountId;
    QVariantMap args;

    argument.beginStructure();

    argument >> displayName
             >> userName
             >> methodId
             >> shareUiPath
             >> capabilities
             >> accountId
             >> iconUrl
             >> args;

    argument.endStructure();

    sm.setDisplayName(displayName);
    sm.setUserName(userName);
    sm.setMethodId(methodId);
    sm.setShareUiPath(shareUiPath);
    sm.setCapabilities(capabilities);
    sm.setAccountId(accountId);
    sm.setIconUrl(QUrl(iconUrl));
    sm.setArgs(args);

    return argument;
}

QDebug operator<<(QDebug dbg, const SharingMethod &sm)
{
    QDebugStateSaver saver(dbg);
    Q_UNUSED(saver)
    dbg.nospace() << "SharingMethod(";
    dbg << "Display Name: " << sm.displayName();
    dbg << ", User Name: " << sm.userName();
    dbg << ", Method ID: " << sm.methodId();
    dbg << ", Share UI Path: " << sm.shareUiPath();
    dbg << ", Capabilites: " << sm.capabilites();
    dbg << ", Icon URL: " << sm.iconUrl();
    dbg << ", Args: " << sm.args();
    dbg << ')';
    return dbg.maybeSpace();
}
