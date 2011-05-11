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

namespace Kapotah
{
    struct TransferFile;
}

/** @brief Displays recieved & sending messages and peer status.
 * 
 * This class displays the sending & recieving messages, peer status i.e. whether peer is
 * online/offline/is-typing-any-text.It sends the new message typed to the MessageDispatcher
 * so that the new message typed can be sent to the friend peer.
 */
class ChatDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit ChatDialog (const QPersistentModelIndex &ipAddress, QWidget *parent = 0, Qt::WindowFlags f = 0);

    private:
        Ui::ChatDialog ui;
        QHostAddress m_peerIp;
        QTimer m_isTypingTimer;
        void sendFiles(QList<Kapotah::TransferFile> fileList);

    private slots:
        
        /** If a new message is recieved with the IP same as that of the IP of opened ChatDialog,
         * it displays the message.
         * If ChatDialog not in focus, it notifies the Notifications about the arrival of new message.
         * @param message       message to be sent
         * @param peerAddress   address of peer for whom the message was meant
         */
        void displayRecievedMessage (QString message, QHostAddress peerAddress);
        void displaySendingMessage();
        
        /** It takes the plain text from the UI and sends the new typed message along with the IP address
         * of the user to MessageDispatcher so it can be sent to friend peer
         */
        void sendNewMessage();
        void displayPeerStatus(QHostAddress);
        void clearStatus();
        void userOffline(const QHostAddress &address);
        void userBackOnline(const QHostAddress &address);
        void sendFileNeedsSourceDir();

    public slots:
        
        /**Whenever a new notification for message is shown and the user clicks on the notification,
         * this function shows the ChatDialog along with the new message just arrived.
         */
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

