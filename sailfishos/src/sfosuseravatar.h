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

#ifndef SFOSUSERAVATAR_H
#define SFOSUSERAVATAR_H

#include <QObject>
#include "../../common/useravatar.h"
#include "../../common/globals.h"

/*!
 * \brief Requests and stores the user’s avatar image.
 *
 * This reimplements UserAvatar to set a default avatar image that
 * fits the SailfishOS icon theme.
 *
 * \since 0.7.0
 */
class SfosUserAvatar : public UserAvatar
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new %SfosUserAvatar object with the given \a parent.
     */
    explicit SfosUserAvatar(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the %SfosUserAvatar object.
     */
    ~SfosUserAvatar() override;

protected:
    /*!
     * \brief Returns the URL of the default avatar image.
     */
    QUrl defaultAvatarUrl() const override;

private:
    Q_DISABLE_COPY(SfosUserAvatar)
    Q_DISABLE_MOVE(SfosUserAvatar)
};

#endif // SFOSUSERAVATAR_H
