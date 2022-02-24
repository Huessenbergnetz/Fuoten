/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SFOSNOTIFICATOR_H
#define SFOSNOTIFICATOR_H

#include <Fuoten/Helpers/AbstractNotificator>

class SfosConfig;

class SfosNotificator : public Fuoten::AbstractNotificator
{
    Q_OBJECT
    Q_DISABLE_COPY(SfosNotificator)
    Q_DISABLE_MOVE(SfosNotificator)
public:
    explicit SfosNotificator(SfosConfig *config, QObject *parent = nullptr);

    ~SfosNotificator() override;

    void notify(Fuoten::AbstractNotificator::Type type, QtMsgType severity, const QVariant &data) const override;

private:
    SfosConfig *m_config = nullptr;
    QString m_infoIcon;
    QString m_errorIcon;
};

#endif // SFOSNOTIFICATOR_H
