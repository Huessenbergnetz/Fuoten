/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "languagesmodel.h"

LanguagesModel::LanguagesModel(QObject *parent) :
    Hbnsc::LanguageModel({QStringLiteral("de"),
                         QStringLiteral("da"),
                         QStringLiteral("en_GB"),
                         QStringLiteral("en_US"),
                         QStringLiteral("fr"),
                         QStringLiteral("nl"),
                         QStringLiteral("sv"),
                         QStringLiteral("zh_CN")}, parent)
{

}

LanguagesModel::~LanguagesModel()
{

}

#include "moc_languagesmodel.cpp"
