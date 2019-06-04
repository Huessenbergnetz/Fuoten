/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2019 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/src/main.cpp
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

#ifndef LICENSESMODEL_H
#define LICENSESMODEL_H

#include "hbnsclicensemodel.h"

class LicensesModel : public Hbnsc::LicenseModel
{
    Q_OBJECT
    Q_DISABLE_COPY(LicensesModel)
public:
    explicit LicensesModel(QObject *parent = nullptr);
    ~LicensesModel() override;
};

#endif // LICENSESMODEL_H
