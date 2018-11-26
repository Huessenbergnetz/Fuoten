/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2018 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/src/dbus/fuotendbusproxy.h
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

#ifndef FUOTENDBUSPROYX_H
#define FUOTENDBUSPROYX_H

#include <QObject>

class FuotenDbusProxy : public QObject
{
    Q_OBJECT
public:
    explicit FuotenDbusProxy(QObject *parent = nullptr);

public slots:
    void activate();

signals:
    void dbusActivate();
};

#endif // FUOTENDBUSPROYX_H
