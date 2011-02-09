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
#include <QUrl>
#include <announcer.h>

using namespace Kapotah;

ChatDialog::ChatDialog (const QPersistentModelIndex &ipAddress, QWidget* parent, Qt::WindowFlags f) : QDialog (parent, f)
{
    ui.setupUi (this);
    m_ipAddress = ipAddress;
    setWindowTitle (PeerManager::instance()->peersModel()->data (m_ipAddress, Qt::DisplayRole).toString()
                    + " (" + PeerManager::instance()->peersModel()->data (m_ipAddress, PeersModel::ipAddressRole).toString()
                    + ")");
    connect (MessageDispatcher::instance(), SIGNAL (gotNewMessage (QString, QHostAddress)), this,
             SLOT (displayRecievedMessage (QString, QHostAddress)));
    connect (ui.sendMessage, SIGNAL (pressed()), this, SLOT (sendNewMessage()));
    connect (ui.sendMessage, SIGNAL (pressed()), this, SLOT (displaySendingMessage()));
    connect (ui.sendMessage, SIGNAL (pressed()), ui.messageEdit, SLOT (clear()));

    ui.messageEdit->setFocus();
    ui.messageEdit->installEventFilter (this);

    setAcceptDrops (true);
}


void ChatDialog::displayRecievedMessage (QString message, QHostAddress peerAddress)
{
    if (peerAddress.toString() == PeerManager::instance()->peersModel()->data (m_ipAddress, PeersModel::ipAddressRole).toString()) {
        ui.messageDisplay->appendPlainText (PeerManager::instance()->peersModel()->data (m_ipAddress, Qt::DisplayRole).toString()
                                            + "::" + message);
    }
}

void ChatDialog::displaySendingMessage()
{
    ui.messageDisplay->appendPlainText ("Me:: " + ui.messageEdit->toPlainText());
}

void ChatDialog::sendNewMessage()
{
    QHostAddress address (PeerManager::instance()->peersModel()->data (m_ipAddress, PeersModel::ipAddressRole).toString());
    Kapotah::MessageDispatcher::instance()->sendNewMessage (ui.messageEdit->toPlainText(), address);
}

ChatDialog::~ChatDialog()
{

}

bool ChatDialog::eventFilter (QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        Announcer::instance()->peerStatus(QHostAddress(PeerManager::instance()->peersModel()->data(m_ipAddress,
                                                                                                     PeersModel::ipAddressRole).toString()));//send "is typing datagram"
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
        }

        QHostAddress address (PeerManager::instance()->peersModel()->data (m_ipAddress, PeersModel::ipAddressRole).toString());
        Transfer *transfer = TransferManager::instance()->addTransfer (Transfer::Outgoing, files, 0, 0, 0, address);
        transfer->start();
    }
}

#include "chatdialog.moc"
