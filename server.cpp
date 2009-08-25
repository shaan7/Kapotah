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

#include "server.h"
#include <QDomDocument>
#include <QFile>

Server::Server(QObject *parent) :
        QTcpServer(parent)
{
}

void Server::incomingConnection(int socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readIncomingData()));
}

void Server::readIncomingData()
{
    //Parse the datagram as XML
    QDomDocument document;
    document.setContent(dynamic_cast<QTcpSocket*>(sender())->readAll(), false, 0, 0, 0);
    QDomElement documentElement = document.documentElement();
    QDomNode node = documentElement.firstChild();
    QDomElement action = node.toElement();

    //If data is type message
    if (action.attribute("type") == "message") {
        QDomElement message = action.firstChild().toElement();
        emit messageRecieved(message.attribute("content", "ERROR"),message.attribute("senderName", "unknown"));
    }
    if (action.attribute("type") == "file") {
        QDomElement file = action.firstChild().toElement();
        //TODO
        emit fileRecieved(file.attribute("fileName", "UNKNOWN"), file.attribute("size","0").toInt()
                             , file.attribute("ID","0"), file.attribute("senderName", "unknown"));
        pendingRecieveFiles[file.attribute("ID","0")] = dynamic_cast<QTcpSocket*>(sender());
    }
}

void Server::acceptFileTransfer(QString ID)
{
    if (pendingRecieveFiles.contains(ID)) {
        pendingRecieveFiles.value(ID)->write(ID.toUtf8());
    }
}
