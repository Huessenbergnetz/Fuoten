/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2018 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/src/sfosnotificator.cpp
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

#ifndef SFOSNOTIFICATOR_H
#define SFOSNOTIFICATOR_H

#include <Fuoten/Helpers/AbstractNotificator>

class SfosConfig;

class SfosNotificator : public Fuoten::AbstractNotificator
{
    Q_OBJECT
    Q_DISABLE_COPY(SfosNotificator)
public:
    explicit SfosNotificator(SfosConfig *config, QObject *parent = nullptr);

    ~SfosNotificator();

    void notify(Fuoten::AbstractNotificator::Type type, QtMsgType severity, const QVariant &data) const override;

private:
    SfosConfig *m_config = nullptr;
    QString m_infoIcon;
    QString m_errorIcon;
};

#endif // SFOSNOTIFICATOR_H
