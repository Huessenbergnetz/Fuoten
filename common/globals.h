/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(5, 13, 0)
#define Q_DISABLE_MOVE(Class) \
    Class(const Class &&) Q_DECL_EQ_DELETE;\
    Class &operator=(const Class &&) Q_DECL_EQ_DELETE;
#endif

#endif // GLOBALS_H

