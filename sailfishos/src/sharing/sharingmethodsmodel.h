/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SHARINGMETHODSMODEL_H
#define SHARINGMETHODSMODEL_H

#include <QAbstractListModel>
#include "sharingmethod.h"
#include "globals.h"

class QDBusPendingCallWatcher;

class SharingMethodsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SharingMethodsModel(QObject *parent = nullptr);
    ~SharingMethodsModel() override;

    enum Roles { DisplayName = Qt::UserRole + 1, UserName, MethodId, ShareUiPath, AccountId, IconUrl, Args };

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private slots:
    void populateModel(QDBusPendingCallWatcher *call);

private:
    QList<SharingMethod> m_methods;

    void loadMethods();

    Q_DISABLE_COPY(SharingMethodsModel)
    Q_DISABLE_MOVE(SharingMethodsModel)
};

#endif // SHARINGMETHODSMODEL_H
