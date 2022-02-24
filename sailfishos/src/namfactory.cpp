/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "namfactory.h"
#include <QNetworkAccessManager>
#include <QAbstractNetworkCache>
#include <QCoreApplication>

NamFactory::NamFactory(QAbstractNetworkCache *cache) : m_cache(cache)
{

}

NamFactory::~NamFactory() = default;

QNetworkAccessManager *NamFactory::create(QObject *parent)
{
    auto nam = new QNetworkAccessManager(parent);
    if (m_cache) {
        nam->setCache(m_cache);
    }
    return nam;
}
