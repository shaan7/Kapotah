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

#include "notifications.h"

#include "notificationsdialog.h"
#include "notificationitemwidget.h"

#include <QApplication>
#include <QDesktopWidget>

template<> Notifications *Kapotah::Singleton<Notifications>::m_instance = 0;

Notifications::Notifications() : m_dialog(new NotificationsDialog())
{
    updatePosition();
}

void Notifications::notify (NotificationData data)
{
    m_dialog->notify(data);
    updatePosition();
}

void Notifications::updatePosition()
{
    QRect screenRect = QApplication::desktop()->availableGeometry();
    m_dialog->move(screenRect.right() - m_dialog->width(), screenRect.bottom() - m_dialog->height());
}

void Notifications::hideNotificationsDialog()
{
    m_dialog->hide();
}

void Notifications::showNotificationsDialog()
{
    m_dialog->show();
}

void Notifications::toggleNotificationsDialog()
{
    if (m_dialog->isVisible())
        hideNotificationsDialog();
    else
        showNotificationsDialog();
}

#include "notifications.moc"
