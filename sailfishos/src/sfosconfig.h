/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SFOSCONFIG_H
#define SFOSCONFIG_H

#include "configuration.h"
#include "globals.h"

class SfosConfig : public Configuration
{
    Q_OBJECT
    /*!
     * \brief This property holds \c true if a Push Up menu should be shown on article pages.
     *
     * \par Access functions:
     * \li bool pushUpOnArticle() const
     * \li void setPushUpOnArticle(bool pushUpOnArticle)
     *
     * \par Notifier signal:
     * \li void pushUpOnArticleChanged(bool pushUpOnArticle)
     */
    Q_PROPERTY(bool pushUpOnArticle READ pushUpOnArticle WRITE setPushUpOnArticle NOTIFY pushUpOnArticleChanged)
    /*!
     * \brief This property holds \c true if the search field on list pages should be visible.
     *
     * \par Access functions:
     * \li bool showSearch() const
     * \li void setShowSearch(bool showSearch)
     *
     * \par Notifier signal:
     * \li void showSearchChanged(bool showSearch)
     */
    Q_PROPERTY(bool showSearch READ showSearch WRITE setShowSearch NOTIFY showSearchChanged)
public:
    explicit SfosConfig(QObject *parent = nullptr);

    ~SfosConfig() override;

    QString getPassword() const override;
    void setPassword(const QString &password) override;

    /*!
     * \brief Getter function for the \link Configuration::pushUpOnArticle pushUpOnArticle \endlink property.
     * \sa setPushUpOnArticle(), pushUpOnArticleChanged()
     */
    bool pushUpOnArticle() const;
    /*!
     * \brief Setter function for the \link Configuration::pushUpOnArticle pushUpOnArticle \endlink property.
     * \sa pushUpOnArticle(), pushUpOnArticleChanged()
     */
    void setPushUpOnArticle(bool pushUpOnArticle);

    /*!
     * \brief Returns \c true if the search field on list pages should be visible.
     * \sa setShowSearch(), showSearchChanged()
     */
    bool showSearch() const;

    /*!
     * \brief Set \a showSearch to \c true if the search file on list pages should be visible.
     * \sa showSearch(), showSearchChanged()
     */
    void setShowSearch(bool showSearch);

    QString getUserAgent() const override;

    QString getLoginFlowUserAgent() const override;

signals:
    /*!
     * \brief Notification signal for the \link Configuration::pushUpOnArticle pushUpOnArticle \endlink property.
     * \sa pushUpOnArticle(), setPushUpOnArticle()
     */
    void pushUpOnArticleChanged(bool pushUpOnArticle);

    /*!
     * \brief Emitted when showSearch changed.
     * \sa showSearch(), setShowSearch()
     */
    void showSearchChanged(bool showSearch);

private:
    QString encPw(const QString &pw) const;
    QString decPw(const QString &pw) const;
    QString getHwReleasename() const;

    QString m_password;
    bool m_pushUpOnArticle = false;
    bool m_showSearch = false;

    Q_DISABLE_COPY(SfosConfig)
    Q_DISABLE_MOVE(SfosConfig)
};

#endif // SFOSCONFIG_H
