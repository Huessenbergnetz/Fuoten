/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2019 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * common/updateintervalmodel.h
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

#ifndef UPDATEINTERVALMODEL_H
#define UPDATEINTERVALMODEL_H

#include "globals.h"
#include <QAbstractListModel>
#include <vector>
#include <utility>

/*!
 * \brief Model containing available update intervals.
 */
class UpdateIntervalModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new UpdateIntervalModel with the given \a parent.
     */
    explicit UpdateIntervalModel(QObject *parent = nullptr);

    /*!
     * \brief Destroys the UpdateIntervalModel object.
     */
    ~UpdateIntervalModel() override;

    /*!
     * \brief The model roles.
     *
     * For access from QML, use the enum name starting lowercase.
     */
    enum Roles {
        Interval = Qt::UserRole + 1,    /**< The update interval in seconds */
        Name                            /**< Human readable name of the interval. */
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE int findIndex(quint32 interval);

private:
    std::vector<std::pair<quint32,QString>> m_intervals;

    /*!
     * \brief Clears the model datan and deletes any entry.
     */
    void clear();

    /*!
     * \brief Initializes the model data.
     *
     * This is called directly in the constructor.
     */
    void init();

    Q_DISABLE_COPY(UpdateIntervalModel)
    Q_DISABLE_MOVE(UpdateIntervalModel)
};

#endif // UPDATEINTERVALMODEL_H
