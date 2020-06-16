/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/src/dbus/fuotendbusadaptor.h
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

#ifndef FUOTENDBUSADAPTOR_H
#define FUOTENDBUSADAPTOR_H

#include <QDBusAbstractAdaptor>
#include "../../common/globals.h"

class FuotenDbusAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.harbour.fuoten")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.harbour.fuoten\">\n"
"    <method name=\"activate\" />\n"
"  </interface>\n")

public:
    explicit FuotenDbusAdaptor(QObject *parent);
    ~FuotenDbusAdaptor() override;

public Q_SLOTS:
    Q_NOREPLY void activate();

private:
    Q_DISABLE_COPY(FuotenDbusAdaptor)
    Q_DISABLE_MOVE(FuotenDbusAdaptor)
};

#endif // FUOTENDBUSADAPTOR_H
