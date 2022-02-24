/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef USERAGENTMODEL_H
#define USERAGENTMODEL_H

#include <QAbstractListModel>
#include <vector>
#include <utility>
#include "globals.h"

class UserAgentModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new UserAgentModel with the given \a parent.
     */
    explicit UserAgentModel(QObject *parent = nullptr);

    /*!
     * Destroys the UserAgentModel object.
     */
    ~UserAgentModel() override;

    /*!
     * \brief The model roles.
     *
     * For access from QML, use the enum name starting lowercase.
     */
    enum Roles {
        Title = Qt::UserRole + 1,   /**< The title of the user agent. */
        Agent                       /**< The user agent string itself. */
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    std::vector<std::pair<QString,QString>> m_agents;

    /*!
     * \brief Clears the model data.
     */
    void clear();

    /*!
     * \brief Initializes the model data.
     *
     * This is called directly in the constructor.
     */
    void init();

    Q_DISABLE_COPY(UserAgentModel)
    Q_DISABLE_MOVE(UserAgentModel)
};

#endif // USERAGENTMODEL_H
