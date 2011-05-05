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


#include "chatdialog.h"
#include <peermanager.h>
#include <transfermanager.h>
#include <transfer.h>
#include <announcer.h>
#include <xml/transferxmlparser.h>
#include "notifications/notifications.h"
#include <QUrl>
#include <QFileDialog>

using namespace Kapotah;

ChatDialog::ChatDialog (const QPersistentModelIndex &ipAddress, QWidget* parent, Qt::WindowFlags f)
    : QDialog (parent, f)
{
    ui.setupUi(this);
    m_peerIp=PeerManager::instance()->ipFromIndex(ipAddress);
    setWindowTitle(PeerManager::instance()->nameFromIp(m_peerIp) + " (" + m_peerIp.toString() + ")");
    connect (MessageDispatcher::instance(), SIGNAL(gotNewMessage(QString, QHostAddress)), this,
             SLOT(displayRecievedMessage(QString, QHostAddress)));
    connect (Announcer::instance(), SIGNAL(peerTyping(QHostAddress)), this, SLOT(displayPeerStatus(QHostAddress)));
    connect (ui.sendMessage, SIGNAL(pressed()), this, SLOT(sendNewMessage()));
    connect (ui.sendMessage, SIGNAL(pressed()), this, SLOT(displaySendingMessage()));
    connect (ui.sendMessage, SIGNAL(pressed()), ui.messageEdit, SLOT(clear()));
    connect (PeerManager::instance(), SIGNAL(peerAdded(QHostAddress)), SLOT(userBackOnline(QHostAddress)));
    connect (PeerManager::instance(), SIGNAL(peerRemoved(QHostAddress)), SLOT(userOffline(QHostAddress)));
    connect (ui.sendFileButton, SIGNAL(clicked()), SLOT(sendFileNeedsSourceDir()));

    ui.messageEdit->setFocus();
    ui.messageEdit->installEventFilter (this);
    ui.sendMessage->setToolTip("Send Message");
    ui.sendFileButton->setToolTip("Send file. To send multiple file, use Drag & Drop");

    setAcceptDrops (true);
}

void ChatDialog::displayRecievedMessage(QString message, QHostAddress peerAddress)
{
    if (peerAddress == m_peerIp) {
        QString peerName = PeerManager::instance()->nameFromIp(peerAddress);

        if (QApplication::activeWindow() != this) {
            NotificationData data = {peerName + " says" , message, QIcon(":/images/download.png"), this };
            Notifications::instance()->notify(data);
        }

        if(!message.isEmpty()){
            ui.messageDisplay->appendHtml("<b>" + PeerManager::instance()->nameFromIp(peerAddress) + "</b>" + ":: " + message);
        }
    }
}

void ChatDialog::displayPeerStatus(QHostAddress peerAddress)
{
        connect (&m_isTypingTimer, SIGNAL(timeout()), this, SLOT(clearStatus()));
        m_isTypingTimer.start(2000);
        if(peerAddress == m_peerIp)
        {
            ui.peerStatus->setText(PeerManager::instance()->nameFromIp(m_peerIp) + " is typing....");
        }
}

void ChatDialog::clearStatus()
{
    ui.peerStatus->clear();
}

void ChatDialog::displaySendingMessage()
{
    if(!ui.messageEdit->toPlainText().isEmpty()){
        ui.messageDisplay->appendHtml ("<b>Me::</b> " + ui.messageEdit->toPlainText());
    }
}

void ChatDialog::sendNewMessage()
{
    Kapotah::MessageDispatcher::instance()->sendNewMessage (ui.messageEdit->toPlainText(), m_peerIp);
}

ChatDialog::~ChatDialog()
{

}

bool ChatDialog::eventFilter (QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        Announcer::instance()->peerStatus(m_peerIp);     //send "is typing datagram"
        if ((keyEvent->modifiers()==Qt::NoModifier)  
            && (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) ) {
                ui.sendMessage->animateClick(); //Click the button
                return true;
            } else {
                return false;
            }
    } else {
        return QDialog::eventFilter (obj, event);
    }
}

void ChatDialog::dragEnterEvent (QDragEnterEvent *event)
{
    setBackgroundRole (QPalette::Highlight);
    event->acceptProposedAction();
}

void ChatDialog::dragMoveEvent (QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void ChatDialog::dragLeaveEvent (QDragLeaveEvent*)
{
    setBackgroundRole (QPalette::NoRole);
}

void ChatDialog::dropEvent (QDropEvent* event)
{
    setBackgroundRole (QPalette::NoRole);
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasUrls()) {
        QList<TransferFile> files;
         foreach (QUrl url, mimeData->urls()) {
             TransferFile file;
             file.path = url.toLocalFile();
             files.append (file);
             ui.messageDisplay->appendHtml ("<b>Sent the file</b> " + file.path);
         }
 
         Transfer *transfer = TransferManager::instance()->addTransfer (Transfer::Outgoing, files, m_peerIp, false);
         transfer->start();
     }
}

void ChatDialog::notificationActivated()
{
    show();
    activateWindow();
}

void ChatDialog::userBackOnline(const QHostAddress& address)
{
    if (address == m_peerIp) {
        ui.messageDisplay->appendHtml(address.toString() + " <b><font color=\"green\">is back online</font></b>");
        ui.sendMessage->setEnabled(true);
    }
}

void ChatDialog::userOffline(const QHostAddress& address)
{
    if (address == m_peerIp) {
        ui.messageDisplay->appendHtml(address.toString() + " <b><font color=\"red\">is now offline</font></b>");
        ui.sendMessage->setEnabled(false);
    }
}

void ChatDialog::closeEvent(QCloseEvent* event)
{
    hide();
    event->ignore();
}

void ChatDialog::sendFileNeedsSourceDir()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this, "Select file(s)", QDir::homePath());
    QList<TransferFile> fileList;

    foreach (QString path, filenames) {
        TransferFile file;
        file.path = path;
        fileList.append (file);
        ui.messageDisplay->appendHtml ("<b>Sent the file</b> " + file.path);
    }

    Transfer *transfer = TransferManager::instance()->addTransfer (Transfer::Outgoing, fileList, m_peerIp, false);
    transfer->start();
}

#include "chatdialog.moc"
