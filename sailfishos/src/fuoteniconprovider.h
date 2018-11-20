/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2017 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/src/iconprovider.h
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

#include <QQuickImageProvider>
#include <QPainter>
#include <QColor>
#include <QStringBuilder>

class FuotenIconProvider : public QQuickImageProvider
{
public:
    FuotenIconProvider(const QString &iconsDir) : QQuickImageProvider(QQuickImageProvider::Pixmap)
    {
        m_iconsDir = iconsDir;
    }

    ~FuotenIconProvider() override
    {

    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override
    {
        const QStringList parts = id.split('?', QString::SkipEmptyParts);
        const QString filePath = m_iconsDir % parts.at(0) % QStringLiteral(".png");

        qDebug("Loading image from %s", qUtf8Printable(filePath));

        QPixmap sourcePixmap(filePath, "png");

        if (!sourcePixmap.isNull()) {

            if (size) {
                *size  = sourcePixmap.size();
            }

            if (parts.length() > 1 && QColor::isValidColor(parts.at(1))) {
                QPainter painter(&sourcePixmap);
                painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                painter.fillRect(sourcePixmap.rect(), parts.at(1));
                painter.end();
            }

            if (!requestedSize.isEmpty()) {
                return sourcePixmap.scaled(requestedSize);
            }
        }

        return sourcePixmap;
    }

private:
    QString m_iconsDir;
    Q_DISABLE_COPY(FuotenIconProvider)
};

#endif // FUOTENICONPROVIDER_H
