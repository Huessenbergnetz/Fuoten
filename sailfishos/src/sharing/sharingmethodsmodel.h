/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2019 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/src/sharing/sharingmethodsmodel.h
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

#ifndef SHARINGMETHODSMODEL_H
#define SHARINGMETHODSMODEL_H

#include <QAbstractListModel>
#include "sharingmethod.h"

class SharingMethodsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SharingMethodsModel(QObject *parent = nullptr);

    enum Roles { DisplayName = Qt::UserRole + 1, UserName, MethodId, ShareUiPath, AccountId, IconUrl, Args };

    virtual QHash<int, QByteArray> roleNames() const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList<SharingMethod> m_methods;

    void loadMethods();
};

#endif // SHARINGMETHODSMODEL_H
