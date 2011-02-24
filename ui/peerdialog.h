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
#include "chatdialog.h"
#include "transferdialog.h"
#include <peermanager.h>
#include <QDialog>
#include <QtGui>

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
        
        void createTrayIcon();
        void createActions();
        
        QSystemTrayIcon *trayIcon;
        QMenu *trayIconMenu;
        QAction *minimizeAction;
        QAction *maximizeAction;
        QAction *restoreAction;
        QAction *quitAction;
        
    protected:
        void closeEvent(QCloseEvent *event);

    private slots:
        ChatDialog* createChatDialog (QModelIndex index);
        ChatDialog* createChatDialogOnMessage (QString message, QHostAddress peerAddress);
        void showTransferDialog (bool checked);
        void removeKeyFromHash();
        void iconActivated(QSystemTrayIcon::ActivationReason);
        void quitApplication();
        void updatePeer();
        void updateSystrayTooltip(QModelIndex parent, int start, int end);
};

#endif // PEERDIALOG_H
