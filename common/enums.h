/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Fuoten
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

#ifndef ENUMS_H
#define ENUMS_H

#include <QMetaType>
#include "globals.h"

class FuotenAppEnums {
    Q_GADGET
public:

    enum Context : quint8 {
        StartPage       = 0,
        Folders         = 1,
        Feeds           = 2,
        AllItems        = 3,
        FolderItems     = 4,
        FeedItems       = 5,
        StarredItems    = 6,
        SingleItem      = 7
    };
    Q_ENUM(Context)

    enum OpenIn : quint8 {
        OpenInternal    = 0,
        OpenWebView     = 1,
        OpenExternal    = 2
    };
    Q_ENUM(OpenIn)

private:
    FuotenAppEnums();
    ~FuotenAppEnums();
    Q_DISABLE_COPY(FuotenAppEnums)
    Q_DISABLE_MOVE(FuotenAppEnums)
};

#endif // ENUMS_H
