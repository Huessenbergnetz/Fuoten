/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * common/languagemodel.cpp
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

#include "languagemodel.h"
#include <QLocale>
#include <QStringBuilder>
#include <algorithm>
#include <limits>

LanguageModel::LanguageModel(QObject *parent) :
    QAbstractListModel(parent), m_supportedLangs({QStringLiteral("de"), QStringLiteral("da"), QStringLiteral("en_GB"), QStringLiteral("en_US"), QStringLiteral("fr"), QStringLiteral("nl"), QStringLiteral("sv")})
{
    init();
}



LanguageModel::~LanguageModel()
{
}



QHash<int, QByteArray> LanguageModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Code, QByteArrayLiteral("code"));
    roles.insert(Name, QByteArrayLiteral("name"));
    return roles;
}



int LanguageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    Q_ASSERT(m_langs.size() < std::numeric_limits<int>::max());
    return m_langs.size();
}



QModelIndex LanguageModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    return createIndex(row, column);
}



QVariant LanguageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() > (rowCount()-1)) {
        return QVariant();
    }

    const Language l = m_langs.at(index.row());

    switch(role) {
    case Code:
        return QVariant::fromValue(l.code);
    case Name:
        return QVariant::fromValue(l.name);
    default:
        return QVariant();
    }
}



bool langLessThan(const Language &a, const Language &b)
{
    return (QString::localeAwareCompare(a.name, b.name) < 0);
}



void LanguageModel::init()
{
    clear();

    if (m_supportedLangs.isEmpty()) {
        return;
    }

    beginInsertRows(QModelIndex(), 0, m_supportedLangs.size());

    m_langs.reserve(m_supportedLangs.size() + 1);
    for (const QString &lang : m_supportedLangs) {
        QLocale locale(lang);
        const QString name = locale.nativeLanguageName() % QStringLiteral(" (") % QLocale::languageToString(locale.language()) % QLatin1Char(')');
        m_langs.emplace_back(lang, name);
    }

    std::sort(m_langs.begin(), m_langs.end(), langLessThan);

    //: Means the default language of the system
    //% "Default"
    m_langs.emplace(m_langs.begin(), QString(), qtTrId("id-default-lang"));

    endInsertRows();
}



void LanguageModel::clear()
{
    if (rowCount() == 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), 0, rowCount()-1);

    m_langs.clear();

    endRemoveRows();
}



int LanguageModel::findIndex(const QString &langCode) const
{
    if (rowCount() == 0) {
        return -1;
    }

    int idx = -1;
    int size = static_cast<int>(m_langs.size());
    for (int i = 0; i < size; ++i) {
        if (m_langs.at(i).code == langCode) {
            idx = i;
            break;
        }
    }

    return idx;
}

#include "moc_languagemodel.cpp"
