/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
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

    enum JsSupport: qint8 {
        JsDefault   = -1,
        JsDisabled  =  0,
        JsEnabled   =  1
    };
    Q_ENUM(JsSupport)

private:
    FuotenAppEnums();
    ~FuotenAppEnums();
    Q_DISABLE_COPY(FuotenAppEnums)
    Q_DISABLE_MOVE(FuotenAppEnums)
};

#endif // ENUMS_H
