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

#ifndef USERAVATAR_H
#define USERAVATAR_H

#include <QObject>
#include <QUrl>
#include "../common/globals.h"

/*!
 * \brief Requests and stores the user’s avatar image.
 *
 * After creating this object call check() to either use an already available avatar image file
 * or to request one from the remote server. The image URL will be set to the avatarUrl property.
 * Reimplement this class to select a default image in the defaultAvatarUrl() function.
 *
 * \since 0.7.0
 */
class UserAvatar : public QObject
{
    Q_OBJECT
    /*!
     * \brief This property holds the URL to the user’s avatar image file.
     *
     * The default value is an empty URL.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QUrl</TD><TD>avatarUrl() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>avatarUrlChanged(const QUrl &avatarUrl)</TD></TR></TABLE>
     */
    Q_PROPERTY(QUrl avatarUrl READ avatarUrl NOTIFY avatarUrlChanged)
    /*!
     * \brief Returns \c true while requesting the avatar image is in operation, otherwise \c false.
     *
     * The default valus is \c false. Will be set in the check() function.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>inOperation() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>inOperationChanged(bool inOperation)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool inOperation READ inOperation NOTIFY inOperationChanged)
    /*!
     * \brief This property holds the cache period for the avatar image file in seconds.
     *
     * The default value is 86400 seconds (1 day).
     *
     * \par Access functions:
     * <TABLE><TR><TD>int</TD><TD>cachePeriod() const</TD></TR><TR><TD>void</TD><TD>setCachePeriod(int cachePeriod)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>cachePeriodChanged(int cachePeriod)</TD></TR></TABLE>
     */
    Q_PROPERTY(int cachePeriod READ cachePeriod WRITE setCachePeriod NOTIFY cachePeriodChanged)
public:
    /*!
     * \brief Constructs a new %UserAvatar object with the given \a parent.
     */
    explicit UserAvatar(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the %UserAvatar object.
     */
    ~UserAvatar() override;

    /*!
     * \brief Starts checking for the user’s avatar image.
     *
     * This will check if an avatar image is already available. If one is available and not older than
     * cachePeriod, it’s local file system URL will be set to the avatarUrl property. Otherwise the image
     * will be requested from the remote server using the GetUser API class.
     */
    Q_INVOKABLE void check();

    /*!
     * \brief Returns the URL to the avatar image.
     *
     * The default value is an empty URL.
     *
     * \sa setAvatarUrl(), avatarUrlChanged(), avatarUrl
     */
    QUrl avatarUrl() const;

    /*!
     * \brief Returns \c true while request is in operation, otherwise \c false.
     *
     * The default value is \c false.
     *
     * \sa setInOperation(), inOperationChanged(), inOperation
     */
    bool inOperation() const;

    /*!
     * \brief Returns the avatar image file cache period in seconds.
     *
     * The default value is 86400 seconds (1 day).
     *
     * \sa setCachePeriod(), cachePeriodChanged(), cachePeriod
     */
    int cachePeriod() const;

    /*!
     * \brief Sets the avatar image file cache period in seconds.
     *
     * \sa cachePeriod(), cachePeriodChanged(), cachePeriod
     */
    void setCachePeriod(int cachePeriod);

signals:
    /*!
     * \brief Emitted when the avatarUrl has been changed.
     *
     * This signali will be triggered in setAvatarUrl().
     *
     * \sa avatarUrl(), setAvatarUrl(), avatarUrl
     */
    void avatarUrlChanged(const QUrl &avatarUrl);
    /*!
     * \brief Emitted when inOperation has been changed.
     *
     * This signal will be triggered in setInOperation().
     *
     * \sa inOperation(), setInOperation(), inOperation
     */
    void inOperationChanged(bool inOperation);
    /*!
     * \brief Emitted when the cachePeriod has been changed.
     *
     * This signal will be triggered in setCachePeriod().
     *
     * \sa cachePeriod(), setCachePeriod(), cachePeriod
     */
    void cachePeriodChanged(int cachePeriod);

protected:
    /*!
     * \brief Returns the URL to the default avatar image.
     *
     * By default it returns en empty URL. Reimplement this in a subclass
     * to return a custom default avatar image’s file URL.
     */
    virtual QUrl defaultAvatarUrl() const;

private slots:
    /*!
     * \brief Executed when GetUser successfully requested user data from the remote server.
     *
     * If the returned data contains valid avatar image data, the image will be saved to
     * the local file system. If the returned JSON data does not contain valid image data,
     * setOldOrDefaultAvatarUrl() will be used.
     */
    void gotUser(const QJsonDocument &json);
    /*!
     * \brief Executed when GetUser failed to request user data from the remote server.
     *
     * Will use setOldOrDefaultAvatarUrl() to set the last or default avatar image’s URL.
     */
    void gotUserFailed();

private:
    /*!
     * \brief Sets the avatarUrl and triggers avatarUrlChanged().
     */
    void setAvatarUrl(const QUrl &avatarUrl);
    /*!
     * \brief Sets the default avatar image.
     *
     * Will set the return value of defaultAvatarUrl() to the setAvatarUrl().
     */
    void setDefaultAvatarUrl();
    /*!
     * \brief Set the avatarUrl to an either old or default one.
     *
     * If requesting the avatar image from the remote server fails for
     * some reason, this function will be used to check for available old
     * avatar images. If an old image is found, its URL will be set with
     * setAvatarUrl(), otherwise the return value of defaultAvatarUrl()
     * will be set.
     */
    void setOldOrDefaultAvatarUrl();
    /*!
     * \brief Sets inOperation and triggers inOperationChanged().
     */
    void setInOperation(bool inOperation);

    QUrl m_avatarUrl;
    int m_cachePeriod = 86400;
    bool m_inOperation = false;

    Q_DISABLE_COPY(UserAvatar)
    Q_DISABLE_MOVE(UserAvatar)
};

#endif // USERAVATAR_H
