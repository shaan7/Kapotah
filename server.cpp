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
#include <QNetworkInterface>

static const qint32 BroadcastInterval = 1000;
static const unsigned broadcastPort = 45000;

Server::Server(QObject *parent) :
        QTcpServer(parent)
{
    updateAddresses();  //Updates the entry for broadcasts and IPs of this host
    serverPort = 0;

    broadcastSocket.bind(QHostAddress::Any, broadcastPort, QUdpSocket::ShareAddress
                         | QUdpSocket::ReuseAddressHint);
    connect(&broadcastSocket, SIGNAL(readyRead()),
            this, SLOT(readBroadcast()));

}

void Server::incomingConnection(int socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readIncomingData()));
}

void Server::readIncomingData()     //TCP
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

void Server::sendBroadcast(QByteArray datagram)
{
    bool validBroadcastAddresses = true;
    foreach (QHostAddress address, broadcastAddresses) {
        if (broadcastSocket.writeDatagram(datagram, address,
                                          broadcastPort) == -1)
            validBroadcastAddresses = false;
    }

    if (!validBroadcastAddresses)
        updateAddresses();
}

void Server::readBroadcast()
{
    //Get the datagrams until socket in empty
    while (broadcastSocket.hasPendingDatagrams()) {
        QHostAddress senderIp;
        quint16 senderPort;
        QByteArray datagram;
        datagram.resize(broadcastSocket.pendingDatagramSize());
        if (broadcastSocket.readDatagram(datagram.data(), datagram.size(),
                                         &senderIp, &senderPort) == -1)
            continue;

        emit udpDataRecieved(senderIp,datagram);
    }
}

void Server::updateAddresses()
{
    broadcastAddresses.clear();
    ipAddresses.clear();
    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
        foreach (QNetworkAddressEntry entry, interface.addressEntries()) {
            QHostAddress broadcastAddress = entry.broadcast();
            if (broadcastAddress != QHostAddress::Null &&
                entry.ip() != QHostAddress::LocalHost) {
                broadcastAddresses << broadcastAddress;
                ipAddresses << entry.ip();
            }
        }
    }
}
