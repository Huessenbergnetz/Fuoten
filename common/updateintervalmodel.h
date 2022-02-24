/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
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
