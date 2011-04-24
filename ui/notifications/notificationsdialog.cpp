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

#include "notificationsdialog.h"

#include "notifications.h"
#include "notificationitemwidget.h"

#include <QPropertyAnimation>
#include <QDebug>

static const int s_tabSwitchDelay = 1000;

NotificationsDialog::NotificationsDialog (QWidget* parent, Qt::WindowFlags f)
    : QDialog (parent,f | Qt::FramelessWindowHint)
{
    ui.setupUi(this);
    setFocusPolicy(Qt::NoFocus);
    m_timerId = startTimer(s_tabSwitchDelay);
    connect(ui.tabWidget, SIGNAL(tabCloseRequested(int)), SLOT(removeNotification(int)));
    connect(ui.clearButton, SIGNAL(clicked()), SLOT(clearNotificationsAndClose()));
}

void NotificationsDialog::notify (NotificationData data)
{
    ui.horizontalScrollBar->setMaximum(ui.horizontalScrollBar->maximum()+1);
    NotificationItemWidget *widget = new NotificationItemWidget(data, this);
    ui.tabWidget->addTab(widget, data.icon, data.title);

    if (!isVisible()) {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(1000);
        animation->setStartValue(0);
        animation->setEndValue(1);
        show();
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void NotificationsDialog::timerEvent (QTimerEvent* event)
{
    if (event->timerId() != m_timerId)
        return;

    if (!ui.tabWidget->count())
        return;
    int curIndex = ui.tabWidget->currentIndex();
    if (curIndex == ui.tabWidget->count()-1)
        curIndex = 0;
    else
        curIndex++;

    ui.tabWidget->setCurrentIndex(curIndex);
}

void NotificationsDialog::enterEvent (QEvent* event)
{
    killTimer(m_timerId);
    QWidget::enterEvent(event);
}

void NotificationsDialog::leaveEvent (QEvent* event)
{
    m_timerId = startTimer(s_tabSwitchDelay);
    QWidget::leaveEvent(event);
}

void NotificationsDialog::removeNotification (int index)
{
    qDebug() << "Removing Tab " << index;
    QWidget *page = ui.tabWidget->widget(index);
    ui.tabWidget->removeTab(index);
    ui.horizontalScrollBar->setMaximum(ui.horizontalScrollBar->maximum()-1);
    delete page;
}

void NotificationsDialog::clearNotificationsAndClose()
{

    while (ui.tabWidget->count()) {
        QWidget *page = ui.tabWidget->widget(0);
        ui.tabWidget->removeTab(0);
        ui.horizontalScrollBar->setMaximum(ui.horizontalScrollBar->maximum()-1);
        delete page;
    }
    hide();
}

#include "notificationsdialog.moc"
