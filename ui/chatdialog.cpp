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

using namespace Kapotah;

ChatDialog::ChatDialog (const QPersistentModelIndex &ipAddress, QWidget* parent, Qt::WindowFlags f) : QDialog (parent,f)
{
    ui.setupUi(this);
    m_ipAddress=ipAddress;
    setWindowTitle(PeerManager::instance()->peersModel()->data(m_ipAddress, Qt::DisplayRole).toString()
                   +" ("+PeerManager::instance()->peersModel()->data(m_ipAddress, PeersModel::ipAddressRole).toString()
                   +")");
    connect (MessageDispatcher::instance(), SIGNAL(gotNewMessage(QString, QHostAddress)), this,
             SLOT(displayRecievingMessage(QString, QHostAddress)));
    connect (ui.sendMessage, SIGNAL(pressed()), this, SLOT(sendNewMessage()));
    connect (ui.sendMessage, SIGNAL(pressed()), this, SLOT(displaySendingMessage()));
    connect (ui.sendMessage, SIGNAL(pressed()), ui.messageEdit, SLOT(clear()));
    //connect (ui.messageEdit, SIGNAL(reuturnPressed()), this, SLOT(sendNewMessage()));//TODO::set default enter for messageEdit
}

void ChatDialog::displayRecievingMessage(QString message, QHostAddress peerAddress)
{
        if(peerAddress.toString() == PeerManager::instance()->peersModel()->data(m_ipAddress,PeersModel::ipAddressRole).toString())
        {
            ui.messageDisplay->appendPlainText(PeerManager::instance()->peersModel()->data(m_ipAddress, Qt::DisplayRole).toString()
                                               +"::"+message);
        }
}

void ChatDialog::displaySendingMessage()
{
    ui.messageDisplay->appendPlainText(PeerManager::instance()->peersModel()->data(m_ipAddress, Qt::DisplayRole).toString()
                                       +"::"+ ui.messageEdit->toPlainText());
}

void ChatDialog::sendNewMessage()
{
    QHostAddress address(PeerManager::instance()->peersModel()->data(m_ipAddress, PeersModel::ipAddressRole).toString());
    Kapotah::MessageDispatcher::instance()->sendNewMessage(ui.messageEdit->toPlainText(),address);
}

ChatDialog::~ChatDialog()
{

}

#include "chatdialog.moc"
