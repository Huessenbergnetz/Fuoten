/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "fuotendbusadaptor.h"

FuotenDbusAdaptor::FuotenDbusAdaptor(QObject *parent) : QDBusAbstractAdaptor(parent)
{

}

FuotenDbusAdaptor::~FuotenDbusAdaptor() = default;

void FuotenDbusAdaptor::activate()
{
    qDebug("%s", "Activating main window via D-Bus.");
    const bool ret = QMetaObject::invokeMethod(parent(), "activate");
    Q_ASSERT(ret);
}
