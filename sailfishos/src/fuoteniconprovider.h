/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/src/main.cpp
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

#ifndef FUOTENICONPROVIDER_H
#define FUOTENICONPROVIDER_H

#include <sailfishapp.h>
#include <QQuickImageProvider>
#include <QPainter>
#include <QColor>

class FuotenIconProvider : public QQuickImageProvider
{
public:
    FuotenIconProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap)
    {
    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
    {
        QStringList parts = id.split('?');

        QPixmap sourcePixmap(SailfishApp::pathTo("images/" + parts.at(0) + ".png").toString(QUrl::RemoveScheme));

        if (size) {
            *size  = sourcePixmap.size();
        }

        if (parts.length() > 1) {
            if (QColor::isValidColor(parts.at(1)))
            {
                QPainter painter(&sourcePixmap);
                painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                painter.fillRect(sourcePixmap.rect(), parts.at(1));
                painter.end();
            }
        }

        if (requestedSize.width() > 0 && requestedSize.height() > 0) {
            return sourcePixmap.scaled(requestedSize.width(), requestedSize.height(), Qt::IgnoreAspectRatio);
        } else {
            return sourcePixmap;
        }
    }
};

#endif // FUOTENICONPROVIDER_H
