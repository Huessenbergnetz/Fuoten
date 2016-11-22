/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
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

#include "imagecache.h"
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QImage>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

/*!
 * \brief Constructs a new ImageCache object.
 */
ImageCache::ImageCache(QObject *parent) :
    QObject(parent)
{
    m_sourceUrl = QUrl(QStringLiteral("null"));
    m_nam = nullptr;
    m_reply = nullptr;
    m_inOperation = false;
    m_defaultImageUrl = QUrl(QStringLiteral("image://fuoten/icon-s-feed"));
}


/*!
 * \brief Deconstructs the ImageCache object.
 */
ImageCache::~ImageCache()
{
}


QUrl ImageCache::sourceUrl() const { return m_sourceUrl; }

void ImageCache::setSourceUrl(const QUrl &nSourceUrl)
{
    if (nSourceUrl != m_sourceUrl) {
        m_sourceUrl = nSourceUrl;
#ifdef QT_DEBUG
        qDebug() << "Changed sourceUrl to" << m_sourceUrl;
#endif
        emit sourceUrlChanged(sourceUrl());

        if (!m_sourceUrl.isEmpty()) {
            getImage();
        } else {
            setDefaultImage();
        }
    }
}




QUrl ImageCache::cacheUrl() const { return m_cacheUrl; }

void ImageCache::setCacheUrl(const QUrl &nCacheUrl)
{
    if (nCacheUrl != m_cacheUrl) {
        m_cacheUrl = nCacheUrl;
#ifdef QT_DEBUG
        qDebug() << "Changed cacheUrl to" << m_cacheUrl;
#endif
        emit cacheUrlChanged(cacheUrl());
        setInOperation(false);
    }
}




bool ImageCache::inOperation() const { return m_inOperation; }

void ImageCache::setInOperation(bool nInOperation)
{
    if (nInOperation != m_inOperation) {
        m_inOperation = nInOperation;
#ifdef QT_DEBUG
        qDebug() << "Changed inOperation to" << m_inOperation;
#endif
        emit inOperationChanged(inOperation());
    }
}



QUrl ImageCache::defaultImageUrl() const { return m_defaultImageUrl; }

void ImageCache::setDefaultImageUrl(const QUrl &nDefaultImageUrl)
{
    if (nDefaultImageUrl != m_defaultImageUrl) {
        m_defaultImageUrl = nDefaultImageUrl;
#ifdef QT_DEBUG
        qDebug() << "Changed defaultImageUrl to" << m_defaultImageUrl;
#endif
        emit defaultImageUrlChanged(defaultImageUrl());
    }
}



void ImageCache::setDefaultImage()
{
    setCacheUrl(defaultImageUrl());
    setInOperation(false);
    if (m_reply) {
        delete m_reply;
        m_reply = nullptr;
    }

    if (m_nam) {
        delete m_nam;
        m_nam = nullptr;
    }
}


void ImageCache::getImage()
{
    if (inOperation()) {
        return;
    }

    setInOperation(true);

    if (m_sourceUrl.isEmpty()) {
        qWarning("Source URL empty");
        setDefaultImage();
        return;
    }

    QUrl imageUrl(m_sourceUrl);

    if (!imageUrl.isValid()) {
        qWarning("Invalid image URL");
        setDefaultImage();
        return;
    }

    QString hash(QCryptographicHash::hash(m_sourceUrl.toString().toLatin1(), QCryptographicHash::Md5).toHex());

    m_extension = imageUrl.fileName(QUrl::PrettyDecoded).split(QChar('.'), QString::SkipEmptyParts).last();

    m_filePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation).append(QLatin1String("/")).append(hash);

    if (!m_extension.isEmpty()) {
        m_filePath.append(QLatin1String(".")).append(m_extension);
    }

    if (QFileInfo::exists(m_filePath)) {

        QUrl cu(m_filePath);
        cu.setScheme(QStringLiteral("file"));
        setCacheUrl(cu);

    } else {

        QNetworkRequest request(imageUrl);

#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
        request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, QVariant::fromValue(true));
#endif

        requestImage(request);
    }
}


void ImageCache::requestImage(const QNetworkRequest &request)
{
    if (!m_nam) {
        m_nam = new QNetworkAccessManager(this);
    }

    m_reply = m_nam->get(request);

    connect(m_reply, &QNetworkReply::finished, this, &ImageCache::gotImage);
}



void ImageCache::gotImage()
{
    if (m_reply->error() != QNetworkReply::NoError) {
        qDebug("Netowrk error");
        setDefaultImage();
        return;
    }

    QUrl redirectUrl = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

    if (redirectUrl.isValid()) {

        m_reply->deleteLater();
        m_reply = nullptr;

        QNetworkRequest req(redirectUrl);
        requestImage(req);

    } else {

        QByteArray imgData = m_reply->readAll();

        if (imgData.size() == 0) {
            qWarning("No image data");
            setDefaultImage();
            return;
        }

        if (!saveAsImage(imgData)) {
            if (!saveAsFile(imgData)) {
                qWarning("Failed to save image to cache.");
                setDefaultImage();
                return;
            }
        }

        setCacheUrl(QUrl(m_filePath));

        setInOperation(false);

        m_reply->deleteLater();

        delete m_nam;
    }
}


bool ImageCache::saveAsFile(const QByteArray &data)
{
    QFile f(m_filePath);
    if (!f.open(QIODevice::WriteOnly)) {
        qWarning("Can not open cache file for writing.");
        return false;
    }

    if (f.write(data) < 0) {
        f.close();
        qWarning("Can not write data to file.");
        return false;
    }

    f.close();

    return true;
}


bool ImageCache::saveAsImage(const QByteArray &data)
{
    QImage i;
    if (!i.loadFromData(data)) {
        qWarning("Can not load image data.");
        return false;
    }

    if (i.isNull()) {
        qWarning("No valid image");
        return false;
    }

    if (!i.save(m_filePath)) {
        qWarning("Can not save image.");
        return false;
    }

    return true;
}
