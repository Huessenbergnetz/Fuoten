/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "fuotendbusproxy.h"

FuotenDbusProxy::FuotenDbusProxy(QObject *parent) : QObject(parent)
{

}

FuotenDbusProxy::~FuotenDbusProxy() = default;

void FuotenDbusProxy::activate()
{
    emit dbusActivate();
}
