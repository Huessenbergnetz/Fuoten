/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2017 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * common/updateintervalmodel.cpp
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

#include "updateintervalmodel.h"

UpdateIntervalModel::UpdateIntervalModel(QObject *parent) : QAbstractListModel(parent)
{
    init();
}


UpdateIntervalModel::~UpdateIntervalModel()
{

}


int UpdateIntervalModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_intervals.size();
}


QModelIndex UpdateIntervalModel::index(int row, int column, const QModelIndex &parent) const
{
    QModelIndex idx;

    if (hasIndex(row, column, parent)) {
        idx = createIndex(row, column);
    }

    return idx;
}


QHash<int, QByteArray> UpdateIntervalModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Interval, QByteArrayLiteral("interval"));
    roles.insert(Name, QByteArrayLiteral("name"));
    return roles;
}


QVariant UpdateIntervalModel::data(const QModelIndex &index, int role) const
{
    QVariant var;

    if (index.isValid() && (static_cast<size_t>(index.row()) < m_intervals.size())) {

        const std::pair<quint32,QString> interval = m_intervals.at(index.row());

        switch(role) {
        case Interval:  var.setValue(interval.first);   break;
        case Name:      var.setValue(interval.second);  break;
        default:
            break;
        }

    }

    return var;
}


void UpdateIntervalModel::init()
{
    clear();

    beginInsertRows(QModelIndex(), 0, 7);

    //: automatic update interval
    //% "Disabled"
    m_intervals.push_back(std::make_pair(static_cast<quint32>(0), qtTrId("fuoten-update-interval-disabled")));

    //: automatic update interval
    //% "15 Minutes"
    m_intervals.push_back(std::make_pair(static_cast<quint32>(900), qtTrId("fuoten-update-interval-quarter-hour")));

    //: automatic update interval
    //% "30 Minutes"
    m_intervals.push_back(std::make_pair(static_cast<quint32>(1800), qtTrId("fuoten-update-interval-half-hour")));

    //: automatic update interval
    //% "1 Hour"
    m_intervals.push_back(std::make_pair(static_cast<quint32>(3600), qtTrId("fuoten-update-interval-one-hour")));

    //: automatic update interval
    //% "2 Hours"
    m_intervals.push_back(std::make_pair(static_cast<quint32>(7200), qtTrId("fuoten-update-interval-two-hours")));

    //: automatic update interval
    //% "6 Hours"
    m_intervals.push_back(std::make_pair(static_cast<quint32>(21600), qtTrId("fuoten-update-interval-six-hours")));

    //: automatic update interval
    //% "12 Hours"
    m_intervals.push_back(std::make_pair(static_cast<quint32>(43200), qtTrId("fuoten-update-interval-half-day")));

    //: automatic update interval
    //% "1 Day"
    m_intervals.push_back(std::make_pair(static_cast<quint32>(86400), qtTrId("fuoten-update-interval-one-day")));

    endInsertRows();
}


void UpdateIntervalModel::clear()
{
    if (!m_intervals.empty()) {
        beginRemoveRows(QModelIndex(), 0, m_intervals.size() - 1);
        m_intervals.clear();
        endRemoveRows();
    }
}


int UpdateIntervalModel::findIndex(quint32 interval)
{
    int idx = -1;

    if (!m_intervals.empty()) {
        for (size_t i = 0; i < m_intervals.size(); ++i) {
            if (m_intervals.at(i).first == interval) {
                idx = static_cast<int>(i);
                break;
            }
        }
    }

    return idx;
}

#include "moc_updateintervalmodel.cpp"
