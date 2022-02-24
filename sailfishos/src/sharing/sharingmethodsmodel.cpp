/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "sharingmethodsmodel.h"
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMetaType>
#include <QDBusPendingCall>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QStringList>
#include <QDebug>

SharingMethodsModel::SharingMethodsModel(QObject *parent) : QAbstractListModel(parent)
{
    qDBusRegisterMetaType<SharingMethod>();
    qDBusRegisterMetaType<QList<SharingMethod>>();

    loadMethods();
}


SharingMethodsModel::~SharingMethodsModel()
{

}


QHash<int, QByteArray> SharingMethodsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

    roles.insert(DisplayName, QByteArrayLiteral("displayName"));
    roles.insert(UserName, QByteArrayLiteral("userName"));
    roles.insert(MethodId, QByteArrayLiteral("methodId"));
    roles.insert(ShareUiPath, QByteArrayLiteral("shareUiPath"));
    roles.insert(AccountId, QByteArrayLiteral("accountId"));
    roles.insert(IconUrl, QByteArrayLiteral("iconUrl"));
    roles.insert(Args, QByteArrayLiteral("args"));

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
        case IconUrl:       var.setValue(sm.iconUrl());     break;
        case Args:          var.setValue(sm.args());        break;
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
    QDBusPendingCall call = interface.asyncCall(QStringLiteral("transferMethods2"));
    auto watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &SharingMethodsModel::populateModel);
}

void SharingMethodsModel::populateModel(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<QList<SharingMethod>> reply = *call;

    if (Q_UNLIKELY(!reply.isValid())) {
        qWarning("Failed to query sharing methods via D-Bus: %s", qUtf8Printable(reply.error().message()));
    } else {

        const QList<SharingMethod> _methods = reply.value();
        if (Q_LIKELY(!_methods.empty())) {

            qDebug("%s", "Loaded sharing methods.");
#ifndef QT_NO_DEBUG_OUTPUT
            for (const SharingMethod &m : _methods) {
                qDebug() << m;
            }
#endif
            qDebug("%s", "Filtering methods by text/x-url");
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
            qWarning("%s", "No sharing methods received from D-Bus.");
        }
    }

    qDebug("%s", "Adding clipboard sharing method");
    SharingMethod clipboard;
    clipboard.setAccountId(0);
    clipboard.setMethodId(QStringLiteral("clipboard"));
    clipboard.setIconUrl(QUrl(QStringLiteral("image://theme/icon-m-clipboard")));
    //: name for the pseudo sharing method to copy a link to the clipboard
    //% "Clipboard"
    clipboard.setDisplayName(qtTrId("fuoten-clipboad-sharing"));
    beginInsertRows(QModelIndex(), m_methods.size(), m_methods.size());
    m_methods.push_back(clipboard);
    endInsertRows();
}

#include "moc_sharingmethodsmodel.cpp"
