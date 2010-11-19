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
#include "../messagedispatcher.h"

ChatDialog::ChatDialog (const QPersistentModelIndex &ipAddress, QWidget* parent, Qt::WindowFlags f) : QDialog (parent,f)
{
    ui.setupUi(this);
    m_ipAddress=ipAddress;
    connect (MessageDispatcher, SIGNAL(gotNewMessage()), this, SLOT(displayRecievingMessage());
    connect (ui.sendMessage, SIGNAL(pressed()), this, SLOT(sendNewMessage()));
    connect (ui.sendMessage, SIGNAL(pressed()), this, SLOT(displaySendingMessage()));
}

//void ChatDialog::displaySenderMessage()

ChatDialog::~ChatDialog()
{

}

