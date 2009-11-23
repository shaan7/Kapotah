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

#include "messagesender.h"
#include <QDomDocument>
#include <QTcpSocket>

MessageSender::MessageSender(Pointers *ptr, QObject *parent) : QObject(parent)
{
    manager = ptr->manager;
}

void MessageSender::sendMessage(QString message, PeerInfo destinationPeer)
{
    QDomDocument document;

    QDomElement doc = document.createElement("document");
    QDomElement action = document.createElement("action");
    action.setAttribute( "type", "message" );
    QDomElement msg = document.createElement("message");
    msg.setAttribute("senderName", manager->username());
    msg.setAttribute("content", message);

    document.appendChild(doc);
    doc.appendChild(action);
    action.appendChild(msg);

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_5);
    stream << (quint64)document.toByteArray().size();
    data.append(document.toByteArray());

    QTcpSocket socket;
    socket.abort();
    socket.connectToHost(destinationPeer.ipAddress(), 9876);
    socket.waitForConnected();
    socket.write(data);
    socket.waitForBytesWritten();
    socket.disconnectFromHost();

    if (socket.state()!=QTcpSocket::UnconnectedState)
        socket.waitForDisconnected();

    deleteLater();
}
