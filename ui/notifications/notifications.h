/*
    This file is part of the Kapotah project.
    Copyright (C) 2011 Shantanu Tushar <jhahoneyk@gmail.com>
    Copyright (C) 2011 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include "singleton.h"

#include <QIcon>

class NotificationsDialog;

struct NotificationData
{
    QString title;
    QString message;
    QIcon icon;
    QObject* handler;
};

class Notifications : public Kapotah::Singleton<Notifications>
{
    Q_OBJECT

public:
    explicit Notifications();
    void notify (NotificationData data);

private:
    QList<NotificationData> m_notificationData;
    NotificationsDialog *m_dialog;
    bool m_hasPendingNotifications;

public slots:
    void updatePosition();
    void showNotificationsDialog();
    void hideNotificationsDialog();
    void toggleNotificationsDialog();
};


#endif // NOTIFICATIONS_H
