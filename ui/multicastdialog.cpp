/*
    This file is part of the Kapotah project.
    Copyright (C) 2011 Shantanu Tushar <jhahoneyk@gmail.com>
    Copyright (C) 2011 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

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

#include "multicastdialog.h"
#include "peermanager.h"
#include "transfermanager.h"
#include "messagedispatcher.h"
#include <xml/transferxmlparser.h>
#include <QUrl>
#include <QModelIndex>

using namespace Kapotah;

MulticastDialog::MulticastDialog(QWidget* parent, Qt::WindowFlags f) : QDialog (parent, f)
{
    ui.setupUi(this);
    setWindowTitle("Multicast");
    connect (ui.sendMessage, SIGNAL(pressed()), this, SLOT(sendMessage()));
    ui.sendMessage->setToolTip("Send Message");
    ui.sendFileButton->setToolTip("Send file. To send multiple files, use Drag & Drop");
    ui.peersList->setModel(PeerManager::instance()->peersModel());

    setAcceptDrops (true);
}

void MulticastDialog::sendMessage()
{
    foreach (QModelIndex index, ui.peersList->selectionModel()->selectedIndexes()) {
        QHostAddress address (PeerManager::instance()->peersModel()->data (index, PeersModel::ipAddressRole).toString());
        MessageDispatcher::instance()->sendNewMessage (ui.messageEdit->toPlainText(), address);
    }
    accept();
}

bool MulticastDialog::eventFilter (QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
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

void MulticastDialog::dragEnterEvent (QDragEnterEvent *event)
{
    setBackgroundRole (QPalette::Highlight);
    event->acceptProposedAction();
}

void MulticastDialog::dragMoveEvent (QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void MulticastDialog::dragLeaveEvent (QDragLeaveEvent*)
{
    setBackgroundRole (QPalette::NoRole);
}

void MulticastDialog::dropEvent (QDropEvent* event)
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

     foreach (QModelIndex index, ui.peersList->selectionModel()->selectedIndexes()) {
        QHostAddress address (PeerManager::instance()->peersModel()->data (index, PeersModel::ipAddressRole).toString());
        Transfer *transfer = TransferManager::instance()->addTransfer (Transfer::Outgoing, files, address, false);
        qDebug()<<transfer;
        transfer->start();
        }
     }
     accept();
}


#include "multicastdialog.moc"
