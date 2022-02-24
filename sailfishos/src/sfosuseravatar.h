/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SFOSUSERAVATAR_H
#define SFOSUSERAVATAR_H

#include <QObject>
#include "useravatar.h"
#include "globals.h"

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
