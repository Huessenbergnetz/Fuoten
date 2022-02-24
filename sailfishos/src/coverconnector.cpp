/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "coverconnector.h"

CoverConnector::CoverConnector(QObject *parent) : QObject(parent)
{

}

CoverConnector::~CoverConnector()
{

}

Fuoten::Article *CoverConnector::article() const { return m_article; }

void CoverConnector::setArticle(Fuoten::Article *article)
{
    if (m_article != article) {
        m_article = article;
        qDebug("Article changed to %p.", m_article);
        emit articleChanged(m_article);
    }
}

#include "moc_coverconnector.cpp"
