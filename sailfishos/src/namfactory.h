/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef NAMFACTORY_H
#define NAMFACTORY_H

#include <QObject>
#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkAccessManager>
#include "globals.h"

class QAbstractNetworkCache;

class SfosNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit SfosNetworkAccessManager(QObject *parent = nullptr);
    ~SfosNetworkAccessManager() override;

protected:
    QNetworkReply* createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &originalReq, QIODevice *outgoingData = nullptr) override;

private:
    Q_DISABLE_COPY(SfosNetworkAccessManager)
};

class NamFactory : public QQmlNetworkAccessManagerFactory
{
    Q_DISABLE_COPY(NamFactory)
    Q_DISABLE_MOVE(NamFactory)
public:
    explicit NamFactory(QAbstractNetworkCache *cache);

    ~NamFactory() override;

    QNetworkAccessManager *create(QObject *parent = nullptr) override;

private:
    QAbstractNetworkCache *m_cache = nullptr;
};

#endif // NAMFACTORY_H
