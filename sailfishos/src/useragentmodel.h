/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2018 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/src/useragentmodel.h
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

#ifndef USERAGENTMODEL_H
#define USERAGENTMODEL_H

#include <QAbstractListModel>
#include <vector>
#include <utility>

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
    ~UserAgentModel();

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
};

#endif // USERAGENTMODEL_H
