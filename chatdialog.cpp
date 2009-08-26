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

#include "chatdialog.h"
#include "messagesender.h"
#include "filesenderthread.h"
#include "filerecieverthread.h"
#include <QFileDialog>
#include <QDir>

ChatDialog::ChatDialog(QString peer, PeerManager *peerManager, Server *serverPtr, FileServer *fserverPtr, QWidget *parent)
        : QDialog(parent)
{
    ui.setupUi(this);
    peerName = peer;
    manager = peerManager;
    server = serverPtr;
    fserver = fserverPtr;
    setWindowTitle(peer + "@" + manager->peerInfo(peer).ipAddress().toString());    //Set the window title to peer@ipaddress
    connect(ui.sendButton, SIGNAL(clicked()), this, SLOT(sendMessage()));
    connect(ui.fileButton, SIGNAL(clicked()), this, SLOT(sendFile()));
    connect(ui.cancelFileButton, SIGNAL(clicked()), this, SLOT(cancelFileTransfer()));
    connect(ui.saveFileButton, SIGNAL(clicked()), this, SLOT(saveFile()));
    connect(server, SIGNAL(messageRecieved(QString,QString)), this, SLOT(messageRecieved(QString,QString)));
    connect(server, SIGNAL(fileRecieved(QString,qint64,QString,QString)), this, SLOT(fileRecieved(QString,qint64,QString,QString)));
}

void ChatDialog::sendMessage()
{
    MessageSender *sender = new MessageSender(manager, this);
    sender->sendMessage(ui.messageEdit->text(),manager->peerInfo(peerName));
    ui.chatEdit->append("<b>" + manager->username() + "</b> :: " + ui.messageEdit->text());
    ui.messageEdit->clear();
}

void ChatDialog::sendFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select a file");
    if (filename=="")
        return;
    ui.chatEdit->append("<b>" + manager->username() + "</b> sends a file " + filename);
    FileSenderThread *sender = new FileSenderThread(manager, fserver, filename, manager->peerInfo(peerName));
    sender->run();
}

void ChatDialog::saveFile()
{
    QString filename = QFileDialog::getSaveFileName(this, "Select a file to save", QDir::home().absoluteFilePath(ui.fileNameEdit->text()));
    if (filename=="")
        return;
    ui.fileTransferProgress->setMaximum(ui.fileSizeEdit->text().toInt());
    reciever = new FileRecieverThread(manager, ui.IDEdit->text(), ui.fileSizeEdit->text().toInt(), peerName, filename, this);
    connect(reciever, SIGNAL(progress(int)), ui.fileTransferProgress, SLOT(setValue(int)));
    reciever->run();
}

void ChatDialog::cancelFileTransfer()
{
    reciever->quit();
}

void ChatDialog::messageRecieved(QString message, QString username)
{
    if (username == manager->peerInfo(peerName).name()) {
        ui.chatEdit->append("<b>" + username + "</b> :: " + message);
    }
}

void ChatDialog::fileRecieved(QString filename, qint64 size, QString ID, QString username)
{
    if (username == manager->peerInfo(peerName).name()) {
        ui.fileNameEdit->setText(filename);
        if(size < 1024)
            ui.fileSizeEdit->setText(QString::number(float(size)) + "bytes");
        else if(size >= 1024 && size<(1024*1024))
            ui.fileSizeEdit->setText(QString::number(float(size)/1024) + "kb");
        else if(size >= (1024*1024))
            ui.fileSizeEdit->setText(QString::number(float(size)/(1024*1024)) + "mb");
        ui.IDEdit->setText(ID);
        ui.tabWidget->setCurrentWidget(ui.tabFileTransfer);
    }
}
ChatDialog::~ChatDialog()
{
}
