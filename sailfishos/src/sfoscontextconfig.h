/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SFOSCONTEXTCONFIG_H
#define SFOSCONTEXTCONFIG_H

#include "contextconfig.h"
#include <QObject>

class SfosContextConfig : public ContextConfig
{
    Q_OBJECT
public:
    explicit SfosContextConfig(QObject *parent = nullptr);
    ~SfosContextConfig() override;
};

#endif // SFOSCONTEXTCONFIG_H
