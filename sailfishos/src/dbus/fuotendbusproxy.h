/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef FUOTENDBUSPROYX_H
#define FUOTENDBUSPROYX_H

#include <QObject>
#include "globals.h"

class FuotenDbusProxy : public QObject
{
    Q_OBJECT
public:
    explicit FuotenDbusProxy(QObject *parent = nullptr);

    ~FuotenDbusProxy() override;

public slots:
    void activate();

signals:
    void dbusActivate();

private:
    Q_DISABLE_COPY(FuotenDbusProxy)
    Q_DISABLE_MOVE(FuotenDbusProxy)
};

#endif // FUOTENDBUSPROYX_H
