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


#include "peerdialog.h"
#include "peermanager.h"

using namespace Kapotah;

PeerDialog::PeerDialog (QDialog* parent) : QDialog (parent), m_transferDialog(0)
{
    ui.setupUi(this);
    setWindowTitle("Kapotah");
    ui.peersListView->setModel(PeerManager::instance()->peersModel());
    connect (ui.peersListView, SIGNAL(doubleClicked(QModelIndex)), SLOT(createChatDialog(QModelIndex)));
    connect (MessageDispatcher::instance(), SIGNAL(gotNewMessage(QString, QHostAddress)),
             SLOT(createChatDialogOnMessage(QString,QHostAddress)));
    connect(ui.transferButton, SIGNAL(clicked(bool)), SLOT(showTransferDialog(bool)));
}

ChatDialog* PeerDialog::createChatDialog(QModelIndex index)
{
    PeersModel *model = PeerManager::instance()->peersModel();
    if(!m_openChatDialogs.contains(model->data(index, PeersModel::ipAddressRole).toString()))
    {
        qDebug() << "2" << index.row();
        ChatDialog *chatDlg = new ChatDialog(index);
        chatDlg->show();
        m_openChatDialogs.insert(model->data(index, PeersModel::ipAddressRole).toString(), chatDlg);
    }
}

ChatDialog* PeerDialog::createChatDialogOnMessage(QString message, QHostAddress peerAddress)
{
    if (m_openChatDialogs.contains(peerAddress.toString())) //Already created
        return m_openChatDialogs[peerAddress.toString()];

    PeersModel *model = PeerManager::instance()->peersModel();
    
    QModelIndexList list = model->match(model->index(0), PeersModel::ipAddressRole, peerAddress.toString());
    qDebug() << "0" << list.at(0).row();
    if (list.count() == 0) {
        qDebug() << "NOOOO! someone sent message after being offline, huh?";
    } else {
        qDebug() << "1" << list.at(0).row();
        createChatDialog(list.at(0))->displayRecievedMessage(message, peerAddress);
    }
}

void PeerDialog::showTransferDialog (bool checked)
{
    if (!m_transferDialog) {
        m_transferDialog = new TransferDialog(this);
    }
    m_transferDialog->show();
}

PeerDialog::~PeerDialog()
{

}

#include "peerdialog.moc"
