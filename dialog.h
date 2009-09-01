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
#include "peermanager.h"
#include <QListWidgetItem>
#include "chatdialog.h"
#include "server.h"
#include "fileserver.h"

namespace Ui
{
    class DialogClass;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(Server *server,FileServer *fserver,QWidget *parent = 0);
    ~Dialog();

private:
    Ui::DialogClass *ui;
    PeerManager *manager;
    QHash<QString,ChatDialog*> openChatDialogs;
    Server *m_server;
    FileServer *m_fserver;

private slots:
    void startPeerManager();
    void updateNumOfPeers();
    void addNewPeer(QString peer);
    void removePeer(QString peer);
    ChatDialog* openChatWindow(QListWidgetItem*);
    void showFilesDialog();

public slots:
    void unregisterChatDialog();
    void messageRecieved(QString message,QString username);
};

#endif // DIALOG_H
