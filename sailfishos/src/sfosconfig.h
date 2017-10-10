/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/src/sfosconfig.h
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

#ifndef SFOSCONFIG_H
#define SFOSCONFIG_H

#include "../../common/configuration.h"

class SfosConfig : public Configuration
{
    Q_OBJECT
public:
    explicit SfosConfig(QObject *parent = nullptr);

    ~SfosConfig();

    QString getPassword() const override;
    void setPassword(const QString &password) override;

    QString getUserAgent() const override;

private:
    QString encPw(const QString &pw) const;
    QString decPw(const QString &pw) const;

    QString m_password;
};

#endif // SFOSCONFIG_H
