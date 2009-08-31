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

#include <QDomDocument>
#include <QTimer>
#include "chatdialog.h"
#include "messagesender.h"
#include "filesenderthread.h"
#include "dirsenderthread.h"
#include "filerecieverthread.h"
#include "dirrecieverthread.h"
#include <QFileDialog>
#include <QDir>

ChatDialog::ChatDialog(QString peer, PeerManager *peerManager, Server *serverPtr, FileServer *fserverPtr, QWidget *parent)
        : QDialog(parent), peerName(peer), manager(peerManager), server(serverPtr), fserver(fserverPtr)
{
    ui.setupUi(this);

    setWindowTitle(peer + "@" + manager->peerInfo(peer).ipAddress().toString());    //Set the window title to peer@ipaddress

    connect(ui.sendButton, SIGNAL(clicked()), this, SLOT(sendMessage()));
    connect(ui.fileButton, SIGNAL(clicked()), this, SLOT(sendFile()));
    connect(ui.cancelFileButton, SIGNAL(clicked()), this, SLOT(cancelFileTransfer()));
    connect(ui.saveFileButton, SIGNAL(clicked()), this, SLOT(saveFile()));
    connect(ui.dirButton, SIGNAL(clicked()), this, SLOT(sendDir()));

    connect(server, SIGNAL(messageRecieved(QString,QString)), this, SLOT(messageRecieved(QString,QString)));
    connect(server, SIGNAL(fileRecieved(QString,qint64,QString,QString)), this, SLOT(fileRecieved(QString,qint64,QString,QString)));
    connect(server, SIGNAL(dirRecieved(QDomNodeList,QDomNodeList,QString)), this, SLOT(dirRecieved(QDomNodeList,QDomNodeList,QString)));
    connect(manager, SIGNAL(peerGone(QString)), this, SLOT(checkGonePeer(QString)));
    connect(ui.messageEdit, SIGNAL(textEdited(QString)),this, SLOT(sendStatus()));
    connect(server, SIGNAL(udpDataRecieved(QHostAddress,QByteArray)), this, SLOT(parseUdpDatagram(QHostAddress,QByteArray)));
    connect(&keyStatusTimer, SIGNAL(timeout()), this, SLOT(checkForKeyStatus()));

    keyStatusTimer.setInterval(2000);
}

void ChatDialog::checkGonePeer(QString name)
{
    if (name == peerName)
    {
        ui.chatEdit->append("<font color=red>" + peerName + " is offline</font>");
        ui.sendButton->setEnabled(false);
        ui.fileButton->setEnabled(false);
    }
}

void ChatDialog::sendMessage()
{
    MessageSender *sender = new MessageSender(manager, this);
    sender->sendMessage(ui.messageEdit->text(),manager->peerInfo(peerName));
    ui.chatEdit->append("<b>" + manager->username() + "</b> :: " + ui.messageEdit->text()); //<b> is HTML for bold
    ui.messageEdit->clear();
}

void ChatDialog::sendFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select a file");
    if (filename=="")
        return;
    ui.chatEdit->append("<b>" + manager->username() + "</b> sends a file " + filename);
    FileSenderThread *sender = new FileSenderThread(manager, fserver, filename, manager->peerInfo(peerName),this);
    sender->start();
}

void ChatDialog::saveFile()
{
    QString filename = QFileDialog::getSaveFileName(this, "Select a file to save", QDir::home().absoluteFilePath(ui.fileNameEdit->text()));
    if (filename=="")
        return;
    ui.fileTransferProgress->setMaximum(fileSize);
    reciever = new FileRecieverThread(manager, ui.IDEdit->text(), fileSize, peerName, filename, this);
    connect(reciever, SIGNAL(progress(int)), ui.fileTransferProgress, SLOT(setValue(int)));
    connect(reciever, SIGNAL(done()), this, SLOT(fileTransferComplete()));
    startTime = QDateTime::currentDateTime();
    reciever->start();
}

void ChatDialog::cancelFileTransfer()
{
    reciever->stopTransfer();
}

void ChatDialog::fileTransferComplete()
{
    endTime = QDateTime::currentDateTime();
    int timeTaken = startTime.time().secsTo(endTime.time());
    ui.fileStatusLabel->setText(QString::number((fileSize / timeTaken)/1024) + "KB/s");
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

        fileSize = size;
        if(size<(1024*1024))
            ui.fileSizeEdit->setText(QString::number(float(size)/1024) + " KiB");
        else
            ui.fileSizeEdit->setText(QString::number(float(size)/(1024*1024)) + " MiB");

        ui.IDEdit->setText(ID);
        ui.tabWidget->setCurrentWidget(ui.tabFileTransfer);
    }
}

void ChatDialog::sendStatus()
{
    QDomDocument document;

    QDomElement doc = document.createElement("document");
    QDomElement action = document.createElement("action");
    action.setAttribute( "type", "status" );
    QDomElement status = document.createElement("status");
    status.setAttribute("senderName", manager->username());

    document.appendChild(doc);
    doc.appendChild(action);
    action.appendChild(status);

    server->sendDatagram(document.toByteArray(), manager->peerInfo(peerName).ipAddress());
}

void ChatDialog::parseUdpDatagram(QHostAddress senderIP, QByteArray datagram)
{
    QDomDocument document;
    document.setContent(datagram, false, 0, 0, 0);
    QDomElement documentElement = document.documentElement();
    QDomNode node = documentElement.firstChild();
    QDomElement action = node.toElement();

    //If data is type status
    if (action.attribute("type") == "status") {
        keyStatusTimer.stop();
        QDomElement announce = action.firstChild().toElement();
        PeerInfo tempPeer(announce.attribute("senderName", "unknown"),senderIP);
        ui.buddyStatusLabel->setText(announce.attribute("senderName", "unknown") + " is typing.....");
        keyStatusTimer.start();
    }
}

void ChatDialog::checkForKeyStatus()
{
    ui.buddyStatusLabel->clear();
}

void ChatDialog::dirRecieved(QDomNodeList fileList, QDomNodeList dirList, QString username)
{
    if (username == manager->peerInfo(peerName).name()) {
        QString dirname = QFileDialog::getExistingDirectory(this, "Select a dir");

        DirRecieverThread *reciever = new DirRecieverThread(manager, dirname, fileList, dirList, peerName, this);
        reciever->start();
    }
}

void ChatDialog::sendDir()
{
    QString dirname = QFileDialog::getExistingDirectory(this, "Select a dir");
    if (dirname == "")
        return;
    DirSenderThread *sender = new DirSenderThread(manager, fserver, dirname, manager->peerInfo(peerName), this);
    sender->start();
}

ChatDialog::~ChatDialog()
{
}
