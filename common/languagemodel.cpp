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
#include <algorithm>

LanguageModel::LanguageModel(QObject *parent) :
    QAbstractListModel(parent), m_supportedLangs({QStringLiteral("de"), QStringLiteral("da"), QStringLiteral("en"), QStringLiteral("nl"), QStringLiteral("sv")})
{
    init();
}



LanguageModel::~LanguageModel()
{
    qDeleteAll(m_langs);
    m_langs.clear();
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
    return m_langs.count();
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

    Language *l = m_langs.at(index.row());

    switch(role) {
    case Code:
        return QVariant::fromValue(l->code);
    case Name:
        return QVariant::fromValue(l->name);
    default:
        return QVariant();
    }
}



bool langLessThan(Language *a, Language *b)
{
    return (QString::localeAwareCompare(a->name, b->name) < 0);
}



void LanguageModel::init()
{
    clear();

    if (m_supportedLangs.isEmpty()) {
        return;
    }

    beginInsertRows(QModelIndex(), 0, m_supportedLangs.count()-1);

    Q_FOREACH(const QString &lang, m_supportedLangs) {
        QLocale locale(lang);
        Language *l = new Language;
        l->code = lang;
        l->name = locale.nativeLanguageName();
        l->name.append(QLatin1String(" ("));
        l->name.append(QLocale::languageToString(locale.language()));
        l->name.append(QLatin1String(")"));
        m_langs.append(l);
    }

    std::sort(m_langs.begin(), m_langs.end(), langLessThan);

    Language *defLang = new Language;
    defLang->code = QStringLiteral("");
    //% "Default"
    defLang->name = qtTrId("id-default-lang");
    m_langs.prepend(defLang);

    endInsertRows();
}



void LanguageModel::clear()
{
    if (rowCount() == 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), 0, rowCount()-1);

    qDeleteAll(m_langs);
    m_langs.clear();

    endRemoveRows();
}



int LanguageModel::findIndex(const QString &langCode) const
{
    if (rowCount() == 0) {
        return -1;
    }

    int idx = -1;

    for (int i = 0; i < m_langs.count(); ++i) {
        if (m_langs.at(i)->code == langCode) {
            idx = i;
            break;
        }
    }

    return idx;
}
