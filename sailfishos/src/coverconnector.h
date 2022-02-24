/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef COVERCONNECTOR_H
#define COVERCONNECTOR_H

#include <QObject>
#include "globals.h"

namespace Fuoten {
class Article;
}

class CoverConnector : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Fuoten::Article *article READ article WRITE setArticle NOTIFY articleChanged)
public:
    explicit CoverConnector(QObject *parent = nullptr);

    ~CoverConnector() override;

    Fuoten::Article *article() const;
    void setArticle(Fuoten::Article *article);

signals:
    void articleChanged(Fuoten::Article *article);

private:
    Fuoten::Article *m_article = nullptr;

    Q_DISABLE_COPY(CoverConnector)
    Q_DISABLE_MOVE(CoverConnector)
};

#endif // COVERCONNECTOR_H
