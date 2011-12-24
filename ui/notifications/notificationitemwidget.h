/*
    This file is part of the Kapotah project.
    Copyright (C) 2011 Shantanu Tushar <shaan7in@gmail.com>
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

#ifndef NOTIFICATIONITEMWIDGET_H
#define NOTIFICATIONITEMWIDGET_H

#include "ui_notificationitemwidget.h"

#include <QWidget>
#include "notifications.h"

namespace Ui {
    class NotificationItemWidget;
}


/** @brief Shows the particular notification message.
 * 
 * This class shows the particular notification message like recieved messages,
 * recieved transfers and indexing completed for a particular directory.
 */
class NotificationItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationItemWidget (const NotificationData notificationData, QWidget* parent = 0,
                                     Qt::WindowFlags f = 0);
    void setNotificationData(const NotificationData &data);

private:
    Ui::NotificationItemWidget ui;
    NotificationData m_data;

    void updateItem();
    
private slots:
    void closeNotification();
};

#endif // NOTIFICATIONITEMWIDGET_H
