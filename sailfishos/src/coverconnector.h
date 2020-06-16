/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COVERCONNECTOR_H
#define COVERCONNECTOR_H

#include <QObject>
#include "../common/globals.h"

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
