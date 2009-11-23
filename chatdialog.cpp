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
#include <QFileDialog>
#include <QDir>
#include <QScrollBar>
#include <QFileInfo>
#include <QScrollBar>
#include "chatdialog.h"
#include "messagesender.h"
#include "filesenderthread.h"
#include "dirsenderthread.h"
#include "filerecieverthread.h"
#include "dirrecieverthread.h"

ChatDialog::ChatDialog(QHostAddress peerIP, Pointers *ptr, QWidget *parent)
        : QDialog(parent), peerIP(peerIP), m_ptr(ptr), manager(ptr->manager), server(ptr->server), fserver(ptr->fserver)
{
    ui.setupUi(this);

    setWindowTitle(manager->peerInfo(peerIP.toString()).name() + "@" + peerIP.toString());    //Set the window title to peer@ipaddress

    connect(ui.sendToolButton, SIGNAL(clicked()), this, SLOT(sendMessage()));

    connect(server, SIGNAL(messageRecieved(QString,QHostAddress)), this, SLOT(messageRecieved(QString,QHostAddress)));
    connect(manager, SIGNAL(peerGone(QHostAddress)), this, SLOT(checkGonePeer(QHostAddress)));
    connect(manager, SIGNAL(newPeer(QHostAddress)), this, SLOT(checkPeerReturned(QHostAddress)));
    connect(ui.messageEdit, SIGNAL(textEdited(QString)),this, SLOT(sendStatus()));
    connect(ui.chatEdit, SIGNAL(textChanged()), this, SLOT(scrollToBottom()));
    connect(ui.messageEdit, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
    connect(server, SIGNAL(udpDataRecieved(QHostAddress,QByteArray)), this, SLOT(parseUdpDatagram(QHostAddress,QByteArray)));
    connect(&keyStatusTimer, SIGNAL(timeout()), this, SLOT(checkForKeyStatus()));
    ui.sendToolButton->setToolTip("Send");
    keyStatusTimer.setInterval(2000);
    setAcceptDrops(true);
}

void ChatDialog::dragEnterEvent(QDragEnterEvent *event)
{
    setBackgroundRole(QPalette::Highlight);
    event->acceptProposedAction();
}

void ChatDialog::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void ChatDialog::dropEvent(QDropEvent *event)
{
    setBackgroundRole(QPalette::NoRole);
    const QMimeData *mimeData = event->mimeData();

     if (mimeData->hasImage()) {
     }

     if (mimeData->hasUrls()) {
         QList<QUrl> urlList = mimeData->urls();
         for (int i = 0; i < urlList.size() && i < 32; ++i) {
             if (QFileInfo(urlList.at(i).toLocalFile()).isDir())
             {
                 sendDir(urlList.at(i).toLocalFile());
             }
             else {
                sendFile(urlList.at(i).toLocalFile());
            }
         }
     }
}

void ChatDialog::dragLeaveEvent(QDragLeaveEvent *event)
{
    setBackgroundRole(QPalette::NoRole);
    event->accept();
}

void ChatDialog::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

void ChatDialog::checkGonePeer(QHostAddress IP)
{
    if (IP == peerIP)
    {
        ui.chatEdit->append("<font color=red>" + peerIP.toString() + " is offline</font>");
        ui.sendToolButton->setEnabled(false);
    }
}

void ChatDialog::checkPeerReturned(QHostAddress IP)
{
    if (IP == peerIP)
    {
        ui.chatEdit->append("<font color=green>" + peerIP.toString() + " is back online</font>");
        ui.sendToolButton->setEnabled(true);
    }
}

void ChatDialog::sendMessage()
{
    MessageSender *sender = new MessageSender(m_ptr, this);
    sender->sendMessage(ui.messageEdit->text(),manager->peerInfo(peerIP.toString()));
    ui.chatEdit->append("<b>" + manager->username() + "</b> :: " + ui.messageEdit->text()); //<b> is HTML for bold
    ui.messageEdit->clear();
}

void ChatDialog::sendFile(QString filename)
{
    FileSenderThread *sender = new FileSenderThread(m_ptr, filename, manager->peerInfo(peerIP.toString()),this);
    sender->start();
    ui.chatEdit->append("You're sending a file " + filename);
}

void ChatDialog::messageRecieved(QString message, QHostAddress senderIP)
{
    if (senderIP == peerIP) {
        ui.chatEdit->append("<b>" + manager->peerInfo(senderIP.toString()).name() + "</b> :: " + message);
    }
}

void ChatDialog::scrollToBottom()
{
    ui.chatEdit->verticalScrollBar()->setSliderPosition(ui.chatEdit->verticalScrollBar()->maximum());
}

void ChatDialog::sendStatus()
{
    QDomDocument document;

    QDomElement doc = document.createElement("document");
    QDomElement action = document.createElement("action");
    action.setAttribute("type", "status");
    QDomElement status = document.createElement("status");
    status.setAttribute("senderName", manager->username());

    document.appendChild(doc);
    doc.appendChild(action);
    action.appendChild(status);

    server->sendDatagram(document.toByteArray(), peerIP);
}

void ChatDialog::parseUdpDatagram(QHostAddress senderIP, QByteArray datagram)
{
    if (senderIP!=peerIP)
        return;
    QDomDocument document;
    document.setContent(datagram, false, 0, 0, 0);
    QDomElement documentElement = document.documentElement();
    QDomNode node = documentElement.firstChild();
    QDomElement action = node.toElement();

    //If data is type status
    if (action.attribute("type") == "status") {
        keyStatusTimer.stop();
        QDomElement announce = action.firstChild().toElement();
        ui.buddyStatusLabel->setText(manager->peerInfo(senderIP.toString()).name() + " is typing....");
        keyStatusTimer.start();
    }
}

void ChatDialog::checkForKeyStatus()
{
    ui.buddyStatusLabel->clear();
}

void ChatDialog::sendDir(QString dirname)
{
    DirSenderThread *sender = new DirSenderThread(m_ptr, dirname, manager->peerInfo(peerIP.toString()), this);
    sender->start();
    ui.chatEdit->append("You're sending a directory " + dirname);
}

ChatDialog::~ChatDialog()
{
}

bool ChatDialog::event(QEvent *event)
{
    if (event->type() == QEvent::WindowActivate) {
        emit(activated(peerIP));
        ui.messageEdit->setFocus();
    }
    QDialog::event(event);
    return true;
}
