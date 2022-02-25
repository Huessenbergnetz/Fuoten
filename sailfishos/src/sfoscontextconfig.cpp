/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "sfoscontextconfig.h"
#include "sfosmigrator.h"

SfosContextConfig::SfosContextConfig(QObject *parent) : ContextConfig(SfosMigrator::configFilename(), parent)
{

}

SfosContextConfig::~SfosContextConfig()
{

}

#include "moc_sfoscontextconfig.cpp"
