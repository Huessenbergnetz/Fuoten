/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2018 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/src/coverconnector.cpp
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

#include "coverconnector.h"

CoverConnector::CoverConnector(QObject *parent) : QObject(parent)
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
