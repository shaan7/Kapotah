/*
    This file is part of the Kapotah project.
    Copyright (C) 2009 Shantanu Tushar <jhahoneyk@gmail.com>
    Copyright (C) 2009 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

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


#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include "ui_chatdialog.h"
#include <messagedispatcher.h>

#include <QDialog>
#include <QPersistentModelIndex>

#include <QTimer>

class ChatDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit ChatDialog (const QPersistentModelIndex &ipAddress, QWidget *parent = 0, Qt::WindowFlags f = 0);
        virtual ~ChatDialog();

    private:
        Ui::ChatDialog ui;
        QHostAddress m_peerIp;
        QTimer m_isTypingTimer;

    private slots:
        void displayRecievedMessage (QString message, QHostAddress peerAddress);
        void displaySendingMessage();
        void sendNewMessage();
        void displayPeerStatus(QHostAddress);
        void clearStatus();
        void userOffline(const QHostAddress &address);
        void userBackOnline(const QHostAddress &address);
        void sendFileNeedsSourceDir();

    public slots:
        void notificationActivated();

    protected:
        bool eventFilter (QObject *obj, QEvent *event);
        virtual void dragEnterEvent (QDragEnterEvent *event);
        virtual void dragMoveEvent (QDragMoveEvent *event);
        virtual void dragLeaveEvent (QDragLeaveEvent*);
        virtual void dropEvent (QDropEvent *event);
        virtual void closeEvent (QCloseEvent* event);

        friend class PeerDialog;
        
};

#endif // CHATDIALOG_H

