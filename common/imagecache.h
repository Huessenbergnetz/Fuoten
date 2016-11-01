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

#ifndef IMAGECACHE_H
#define IMAGECACHE_H

#include <QObject>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

/*!
 * \brief The ImageCache class.
 */
class ImageCache : public QObject
{
    Q_OBJECT
    /*!
     * \brief The source URL of the image.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QUrl</TD><TD>sourceUrl() const</TD></TR><TR><TD>void</TD><TD>setSourceUrl(const QUrl &nSourceUrl)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>sourceUrlChanged(const QUrl &sourceUrl)</TD></TR></TABLE>
     */
    Q_PROPERTY(QUrl sourceUrl READ sourceUrl WRITE setSourceUrl NOTIFY sourceUrlChanged)
    /*!
     * \brief The URL of the cached image file.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QUrl</TD><TD>cacheUrl() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>cacheUrlChanged(const QUrl &cacheUrl)</TD></TR></TABLE>
     */
    Q_PROPERTY(QUrl cacheUrl READ cacheUrl NOTIFY cacheUrlChanged)
    /*!
     * \brief Returns true while the image is loaded.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>inOperation() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>inOperationChanged(bool inOperation)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool inOperation READ inOperation NOTIFY inOperationChanged)
    /*!
     * \brief URL/Path to the default image.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QUrl</TD><TD>defaultImageUrl() const</TD></TR><TR><TD>void</TD><TD>setDefaultImageUrl(const QUrl &nDefaultImageUrl)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>defaultImageUrlChanged(const QUrl &defaultImageUrl)</TD></TR></TABLE>
     */
    Q_PROPERTY(QUrl defaultImageUrl READ defaultImageUrl WRITE setDefaultImageUrl NOTIFY defaultImageUrlChanged)
public:
    explicit ImageCache(QObject *parent = nullptr);
    ~ImageCache();

    QUrl sourceUrl() const;
    QUrl cacheUrl() const;
    bool inOperation() const;
    QUrl defaultImageUrl() const;

    void setSourceUrl(const QUrl &nSourceUrl);
    void setDefaultImageUrl(const QUrl &nDefaultImageUrl);


signals:
    void sourceUrlChanged(const QUrl &sourceUrl);
    void cacheUrlChanged(const QUrl &cacheUrl);
    void inOperationChanged(bool inOperation);
    void defaultImageUrlChanged(const QUrl &defaultImageUrl);

private slots:
    void gotImage();

private:
    Q_DISABLE_COPY(ImageCache)
    QUrl m_sourceUrl;
    QUrl m_cacheUrl;
    bool m_inOperation;
    QUrl m_defaultImageUrl;

    QNetworkAccessManager *m_nam;
    QNetworkReply *m_reply;
    QString m_filePath;
    QString m_extension;

    void getImage();
    void calcSize();
    void setDefaultImage();

    void setCacheUrl(const QUrl &nCacheUrl);
    void setInOperation(bool nInOperation);

    bool saveAsImage(const QByteArray &data);
    bool saveAsFile(const QByteArray &data);
    void requestImage(const QNetworkRequest &request);


};

#endif // IMAGECACHE_H
