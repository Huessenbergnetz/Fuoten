/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2017 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/src/sharing/sharingmethod.h
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

#include "sharingmethodsmodel.h"
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMetaType>
#ifndef QT_NO_DEBUG_OUTPUT
#include <QStringList>
#endif

SharingMethodsModel::SharingMethodsModel(QObject *parent) : QAbstractListModel(parent)
{
    qDBusRegisterMetaType<SharingMethod>();
    qDBusRegisterMetaType<QList<SharingMethod>>();

    loadMethods();
}


QHash<int, QByteArray> SharingMethodsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

    roles.insert(DisplayName, QByteArrayLiteral("displayName"));
    roles.insert(UserName, QByteArrayLiteral("userName"));
    roles.insert(MethodId, QByteArrayLiteral("methodId"));
    roles.insert(ShareUiPath, QByteArrayLiteral("shareUiPath"));
    roles.insert(AccountId, QByteArrayLiteral("accountId"));

    return roles;
}


int SharingMethodsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_methods.size();
}


QVariant SharingMethodsModel::data(const QModelIndex &index, int role) const
{
    QVariant var;

    if (index.isValid() && (index.row() < m_methods.size())) {

        const SharingMethod sm = m_methods.at(index.row());

        switch(role) {
        case DisplayName:   var.setValue(sm.displayName()); break;
        case UserName:      var.setValue(sm.userName());    break;
        case MethodId:      var.setValue(sm.methodId());    break;
        case ShareUiPath:   var.setValue(sm.shareUiPath()); break;
        case AccountId:     var.setValue(sm.accountId());   break;
        default:
            break;
        }

    }

    return var;
}


void SharingMethodsModel::loadMethods()
{
    if (!m_methods.empty()) {
        beginRemoveRows(QModelIndex(), 0, m_methods.size() - 1);
        m_methods.clear();
        endRemoveRows();
    }

    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface interface(QStringLiteral("org.nemo.transferengine"), QStringLiteral("/org/nemo/transferengine"), QString(), bus);
    QDBusReply<QList<SharingMethod>> reply = interface.call(QStringLiteral("transferMethods"));

    if (Q_UNLIKELY(!reply.isValid())) {
        qWarning("Failed to query sharing methods via D-Bus: %s", qUtf8Printable(reply.error().message()));
    } else {

        const QList<SharingMethod> _methods = reply.value();
        if (Q_LIKELY(!_methods.empty())) {

            qDebug("Loaded sharing methods.");
#ifndef QT_NO_DEBUG_OUTPUT
            for (const SharingMethod &m : _methods) {
                qDebug("%s; %s; %s; %s; %s", qUtf8Printable(m.displayName()), qUtf8Printable(m.userName()), qUtf8Printable(m.methodId()), qUtf8Printable(m.shareUiPath()), qUtf8Printable(m.capabilites().join(QLatin1Char(','))));
            }
#endif
            qDebug("Filtering methods by text/x-url");
            QList<SharingMethod> _filteredMethods;
            for (int i = 0; i < _methods.size(); ++i) {
                const SharingMethod m = _methods.at(i);
                const QStringList caps = m.capabilites();
                if (caps.contains(QStringLiteral("text/*")) || caps.contains(QStringLiteral("text/x-url")) || caps.contains(QStringLiteral("*"))) {
                    _filteredMethods.push_back(m);
                }
            }

            if (!_filteredMethods.empty()) {
                beginInsertRows(QModelIndex(), 0, _filteredMethods.size() - 1);
                m_methods = _filteredMethods;
                endInsertRows();
            }
        } else {
            qWarning("No sharing methods received from D-Bus.");
        }
    }

    SharingMethod clipboard;
    clipboard.setAccountId(0);
    clipboard.setMethodId(QStringLiteral("clipboard"));
    //: name for the pseudo sharing method to copy a link to the clipboard
    //% "Clipboard"
    clipboard.setDisplayName(qtTrId("fuoten-clipboad-sharing"));
    beginInsertRows(QModelIndex(), m_methods.size(), m_methods.size());
    m_methods.push_back(clipboard);
    endInsertRows();
}

#include "moc_sharingmethodsmodel.cpp"
