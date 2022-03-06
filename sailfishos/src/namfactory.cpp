/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "namfactory.h"
#include <QAbstractNetworkCache>
#include <QCoreApplication>
#include <QNetworkReply>
#include <QNetworkRequest>

SfosNetworkAccessManager::SfosNetworkAccessManager(QObject *parent)
    : QNetworkAccessManager(parent)
{

}

SfosNetworkAccessManager::~SfosNetworkAccessManager()
{

}

QNetworkReply* SfosNetworkAccessManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &originalReq, QIODevice *outgoingData)
{
    auto req = originalReq;
    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    return QNetworkAccessManager::createRequest(op, originalReq, outgoingData);
}

NamFactory::NamFactory(QAbstractNetworkCache *cache) : m_cache(cache)
{

}

NamFactory::~NamFactory() = default;

QNetworkAccessManager *NamFactory::create(QObject *parent)
{
    auto nam = new SfosNetworkAccessManager(parent);
    if (m_cache) {
        nam->setCache(m_cache);
    }

#ifdef QT_DEBUG
    QObject::connect(nam, &QNetworkAccessManager::finished, nam, [](QNetworkReply *reply) {
        const QVariant fromCache = reply->attribute(QNetworkRequest::SourceIsFromCacheAttribute);
        qDebug("%s loaded from cache: %s", qUtf8Printable(reply->url().toString()), fromCache.toBool() ? "true" : "false");
    });
#endif

    return nam;
}

#include "moc_namfactory.cpp"
