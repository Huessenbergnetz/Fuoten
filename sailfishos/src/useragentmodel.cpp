/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2018 Huessenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
 *
 * sailfishos/src/useragentmodel.cpp
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

#include "useragentmodel.h"

UserAgentModel::UserAgentModel(QObject *parent) : QAbstractListModel(parent)
{
    init();
}


UserAgentModel::~UserAgentModel()
{

}


QHash<int, QByteArray> UserAgentModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Title, QByteArrayLiteral("title"));
    roles.insert(Agent, QByteArrayLiteral("agent"));
    return roles;
}


int UserAgentModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_agents.size();
}


QModelIndex UserAgentModel::index(int row, int column, const QModelIndex &parent) const
{
    QModelIndex idx;

    if (hasIndex(row, column, parent)) {
        idx = createIndex(row, column);
    }

    return idx;
}


QVariant UserAgentModel::data(const QModelIndex &index, int role) const
{
    QVariant var;

    if (index.isValid() && (static_cast<size_t>(index.row()) < m_agents.size())) {

        const std::pair<QString,QString> a = m_agents.at(index.row());

        switch (role) {
        case Title: var.setValue(a.first);  break;
        case Agent: var.setValue(a.second); break;
        default:
            break;
        }

    }

    return var;
}


void UserAgentModel::clear()
{
    if (!m_agents.empty()) {

        beginRemoveRows(QModelIndex(), 0, m_agents.size() - 1);

        m_agents.clear();

        endRemoveRows();
    }
}


void UserAgentModel::init()
{
    clear();

    m_agents.reserve(17);

    beginInsertRows(QModelIndex(), 0, 16);

    //: name of a user agent for the internal web view
    //% "Default Jolla Webkit"
    m_agents.push_back(std::make_pair(qtTrId("fuoten-default-jolla-webkit-useragent"), QStringLiteral("Mozilla/5.0 (Maemo; Linux; U; Jolla; Sailfish; like Android) AppleWebKit/538.1 (KHTML, like Gecko) Version/5.1 Chrome/30.0.0.0 Mobile Safari/538.1 (compatible)")));

    //: name of a user agent for the internal web view
    //% "Alternative Jolla Webkit"
    m_agents.push_back(std::make_pair(qtTrId("fuoten-alternative-jolla-webkit-useragent"), QStringLiteral("Mozilla/5.0 (Maemo; Android 4.4.2; U; Jolla; Sailfish; ; rv:38.0) Gecko/38.0 Firefox/38.0 SailfishBrowser/1.0 like Safari/538.1")));

    m_agents.push_back(std::make_pair(QStringLiteral("Jolla Sailfish Browser"), QStringLiteral("Mozilla/5.0 (Maemo; Linux; U; Jolla; Sailfish; Mobile; rv:38.0) Gecko/38.0 Firefox/38.0 SailfishBrowser/1.0 like Safari/538.1")));

    m_agents.push_back(std::make_pair(QStringLiteral("Android 2.2"), QStringLiteral("Mozilla/5.0 (Linux; U; Android 2.2; en-us; Nexus One Build/FRF91) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1")));

    m_agents.push_back(std::make_pair(QStringLiteral("Android 4.4"), QStringLiteral("Mozilla/5.0 (Linux; Android 4.4; Nexus 4 Build/KRT16H) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/30.0.0.0 Mobile Safari/537.36")));

    m_agents.push_back(std::make_pair(QStringLiteral("iPhone 4s"), QStringLiteral("Mozilla/5.0 (iPhone; CPU iPhone OS 5_1 like Mac OS X) AppleWebKit/534.46 (KHTML, like Gecko) Version/5.1 Mobile/9B179 Safari/7534.48.3")));

    m_agents.push_back(std::make_pair(QStringLiteral("N900"), QStringLiteral("Mozilla/5.0 (X11; U; Linux armv7l; en-GB; rv:1.9.2b6pre) Gecko/20100318 Firefox/3.5 Maemo Browser 1.7.4.8 RX-51 N900")));

    m_agents.push_back(std::make_pair(QStringLiteral("N9"), QStringLiteral("Mozilla/5.0 (MeeGo; NokiaN9) AppleWebKit/534.13 (KHTML, like Gecko) NokiaBrowser/8.5.0 Mobile Safari/534.13")));

    m_agents.push_back(std::make_pair(QStringLiteral("Fennec (Firefox Mobile) 9"), QStringLiteral("Mozilla/5.0 (Maemo; Linux armv7l; rv:9.0) Gecko/20111216 Firefox/9.0 Fennec/9.0")));

    m_agents.push_back(std::make_pair(QStringLiteral("Internet Explorer Mobile 9"), QStringLiteral("Mozilla/5.0 (compatible; MSIE 9.0; Windows Phone OS 7.5; Trident/5.0; IEMobile/9.0)")));

    m_agents.push_back(std::make_pair(QStringLiteral("Opera Mini 9"), QStringLiteral("Opera/9.80 (J2ME/MIDP; Opera Mini/9 (Compatible; MSIE:9.0; iPhone; BlackBerry9700; AppleWebKit/24.746; U; en) Presto/2.5.25 Version/10.54")));

    m_agents.push_back(std::make_pair(QStringLiteral("Blackberry 7 (Bold 9900)"), QStringLiteral("Mozilla/5.0 (BlackBerry; U; BlackBerry 9900; en) AppleWebKit/534.11+ (KHTML, like Gecko) Version/7.1.0.346 Mobile Safari/534.11+")));

    m_agents.push_back(std::make_pair(QStringLiteral("Blackberry 10 (Q10)"), QStringLiteral("Mozilla/5.0 (BB10; Kbd) AppleWebKit/537.35+ (KHTML, like Gecko) Version/10.2.0.1803 Mobile Safari/537.35+")));

    m_agents.push_back(std::make_pair(QStringLiteral("NTT Docomo Browser"), QStringLiteral("DoCoMo/2.0 SH901iC(c100;TB;W24H12)")));

    m_agents.push_back(std::make_pair(QStringLiteral("Firefox 43.0 Desktop Version"), QStringLiteral("Mozilla/5.0 (X11; Linux i686; rv:43.0) Gecko/20100101 Firefox/43.0")));

    m_agents.push_back(std::make_pair(QStringLiteral("Chrome 48 Desktop Version"), QStringLiteral("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/48.0.2564.116 Safari/537.36")));

    m_agents.push_back(std::make_pair(QStringLiteral("Google Bot"), QStringLiteral("Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)")));

    endInsertRows();
}

#include "moc_useragentmodel.cpp"
