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

using namespace Kapotah;

MulticastDialog::MulticastDialog(const QModelIndexList &ipAddressList, QWidget* parent, Qt::WindowFlags f) : QDialog (parent, f)
{
    ui.setupUi(this);
    m_ipAddressList=ipAddressList;
    connect (ui.sendMessage, SIGNAL(pressed()), this, SLOT(sendMessage()));
}

void MulticastDialog::sendMessage()
{
    foreach (QModelIndex index, m_ipAddressList) {
        QHostAddress address (PeerManager::instance()->peersModel()->data (index, PeersModel::ipAddressRole).toString());
        MessageDispatcher::instance()->sendNewMessage (ui.messageEdit->toPlainText(), address);
    }
    accept();
}

#include "multicastdialog.moc"
