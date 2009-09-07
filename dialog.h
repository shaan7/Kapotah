/***************************************************************************
 *   Copyright 2009 Shantanu Tushar <jhahoneyk@gmail.com>                  *
 *   Copyright 2009 Sudhendu Roy <sudhendu_roy@yahoo.co.ins>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui/QDialog>
#include <QModelIndex>
#include <QListWidgetItem>
#include "peermanager.h"
#include "chatdialog.h"
#include "server.h"
#include "fileserver.h"
#include "pointers.h"
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QSignalMapper>

namespace Ui
{
    class DialogClass;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(Pointers *ptr,QWidget *parent = 0);
    ~Dialog();

private:
    Ui::DialogClass *ui;
    PeerManager *manager;
    QHash<QString,ChatDialog*> openChatDialogs;
    QHash<QString,QAction*> newMessageActions;
    QSignalMapper newMessageMapper;
    Server *m_server;
    FileServer *m_fserver;
    ChatDialog *chatDlg;
    Pointers *m_ptr;
    void createIcon();
    void createActions();
    void createTrayIcon();
    QSystemTrayIcon *trayIcon;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QMenu *trayIconMenu;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void startPeerManager();
    void updateNumOfPeers();
    void addNewPeer(QString peer);
    void removePeer(QString peer);
    ChatDialog* openChatWindowFromItem(QListWidgetItem*);
    ChatDialog* openChatWindow(QString);
    void showFilesDialog();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void addNewMessageEntry(QString senderName);

public slots:
    void unregisterChatDialog();
    void messageRecieved(QString message,QString username);
};

#endif // DIALOG_H
