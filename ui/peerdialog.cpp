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

PeerDialog::PeerDialog (QDialog* parent) : QDialog (parent), m_transferDialog(0)
{
    ui.setupUi(this);
    setWindowTitle("Kapotah");
    ui.peersListView->setModel(Kapotah::PeerManager::instance()->peersModel());
    connect (ui.peersListView, SIGNAL(doubleClicked(QModelIndex)), SLOT(createChatWindow(QModelIndex)));
    connect(ui.transferButton, SIGNAL(clicked(bool)), SLOT(showTransferDialog(bool)));
}

ChatDialog* PeerDialog::createChatWindow(QModelIndex index)
{
    Kapotah::PeersModel *model = Kapotah::PeerManager::instance()->peersModel();
    if(!openChatDialogs.contains(model->data(index, Kapotah::PeersModel::ipAddressRole).toString()))
    {
        ChatDialog *chatDlg = new ChatDialog(index);
        chatDlg->show();
        openChatDialogs.insert(model->data(index, Kapotah::PeersModel::ipAddressRole).toString(), chatDlg);
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
