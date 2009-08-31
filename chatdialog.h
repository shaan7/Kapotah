/***************************************************************************
 *   Copyright 2009 Shantanu Tushar <jhahoneyk@gmail.com>                  *
 *   Copyright 2009 Sudhendu Roy <sudhendu_roy@yahoo.co.in>                *
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

#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include <QDateTime>
#include "ui_chatdialog.h"
#include "peermanager.h"
#include "server.h"
#include "fileserver.h"
#include "filerecieverthread.h"

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    ChatDialog(QString name, PeerManager *peerManager, Server *serverPtr, FileServer *fserverPtr, QWidget *parent = 0);
    ~ChatDialog();

private:
    Ui::ChatDialog ui;
    QString peerName;
    PeerManager *manager;
    Server *server;
    FileServer *fserver;
    FileRecieverThread *reciever;
    QDateTime startTime;
    QDateTime endTime;
    qint64 fileSize;

private slots:
    void sendMessage();
    void sendFile();
    void saveFile();
    void cancelFileTransfer();
    void fileTransferComplete();
    void sendDir();
    void checkGonePeer(QString name);

public slots:
    void messageRecieved(QString message, QString username);
    void fileRecieved(QString filename,qint64 size,QString ID,QString username);
    void dirRecieved(QDomNodeList fileList, QDomNodeList dirList, QString username);
};

#endif // CHATDIALOG_H
