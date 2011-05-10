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

#include "notificationitemwidget.h"

#include "ui_notificationitemwidget.h"
#include "notifications.h"
#include "notificationsdialog.h"

NotificationItemWidget::NotificationItemWidget (const NotificationData notificationData,
                                                QWidget* parent, Qt::WindowFlags f) : QWidget (parent, f),
                                                m_data(notificationData)
{
    ui.setupUi(this);
    updateItem();

    if (m_data.handler) {
        connect(ui.notificationMessage, SIGNAL(clicked()), m_data.handler, SLOT(notificationActivated()));
    }

    connect(ui.notificationMessage, SIGNAL(clicked()), this, SLOT(closeNotification()));
}

NotificationItemWidget::~NotificationItemWidget()
{

}

QToolButton* NotificationItemWidget::activateButton()
{
    return ui.notificationMessage;
}

void NotificationItemWidget::setNotificationData (const NotificationData& data)
{
    m_data = data;
    updateItem();
}

void NotificationItemWidget::updateItem()
{
    ui.notificationMessage->setText(m_data.message);
    ui.notificationMessage->setIcon(m_data.icon);
}

void NotificationItemWidget::closeNotification()
{
    deleteLater();
}

#include "notificationitemwidget.moc"
