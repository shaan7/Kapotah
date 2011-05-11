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


#ifndef PEERDIALOG_H
#define PEERDIALOG_H

#include "ui_peerdialog.h"


#include <peermanager.h>
#include <QDialog>
#include <QtGui>

class ChatDialog;
class SearchDialog;
class MulticastDialog;
class TransferDialog;
class AboutDialog;

/** @brief Displays the dialog which contains list of peers, address of IP of other subnet and debug messages.
 * 
 * This dialog contains 3 tabs.
 * 1) showing list of peers online
 * 2) settings tab, which contains the IP address of peers from other subnet, button for about dialog
 * and place where user can change its name
 * 3) debug tab for showing debug messages
 * It also has refresh, open-multicast-dialog, open-search-dialog,open-transfer-dialog and open-notifications button.
*/
class PeerDialog : public QDialog
{
        Q_OBJECT

    public:
        PeerDialog (QDialog* parent = 0);
        virtual ~PeerDialog();

    private:
        Ui::PeersDialog ui;
        QHash<QString, ChatDialog*> m_openChatDialogs;
        TransferDialog *m_transferDialog;
        MulticastDialog *m_multicastDialog;
        SearchDialog *m_searchDialog;
        AboutDialog *m_aboutDialog;
        void createTrayIcon();
        void createActions();

        QSystemTrayIcon *m_trayIcon;
        QMenu *m_trayIconMenu;
        QAction *minimizeAction;
        QAction *maximizeAction;
        QAction *restoreAction;
        QAction *quitAction;
        QTimer *m_timer;
        bool m_isGreyScale;

    protected:
        void closeEvent(QCloseEvent *event);

    private slots:
        void setPeerNameFromUi();
        ChatDialog* createChatDialog (QModelIndex index);
        void showChatDialogOnUserRequest(QModelIndex index);
        
        /** It first checks whether the chat dialog with that perticular peerAddress is already created or not.
         *  If not already created, it matches with the index list of the peers, then creates the chat dialog
         *  and displays the new message.
         *  @param message      message recieved from peerAddress
         *  @param peerAddress  IP address for whom the message was meant
         */
        void showChatDialogOnIncomingMessage (QString message, QHostAddress peerAddress);
        void showMulticastDialog();
        void showSearchDialog();
        void showTransferDialog ();
        void iconActivated(QSystemTrayIcon::ActivationReason);
        void quitApplication();
        void updatePeer();
        void updateSystrayTooltip(QModelIndex parent, int start, int end);
        void askUserForNewAdditionalSubnet();
        void removeSelectedAdditionalSubnet();
        void appendDebugMessage(const QString &sender, const QString &message);
        void showAboutDialog();
        void addBroadcastAddress(const QString &address);
};

#endif // PEERDIALOG_H

