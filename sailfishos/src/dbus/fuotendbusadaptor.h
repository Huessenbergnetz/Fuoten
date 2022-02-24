/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef FUOTENDBUSADAPTOR_H
#define FUOTENDBUSADAPTOR_H

#include <QDBusAbstractAdaptor>
#include "globals.h"

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
