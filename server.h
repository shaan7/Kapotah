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

#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QDomNodeList>
#include "serverthread.h"

class Server : public QTcpServer
{
    Q_OBJECT

private:
    //UDP

    QList<QHostAddress> broadcastAddresses;
    QList<QHostAddress> ipAddresses;
    QUdpSocket broadcastSocket;

    void updateAddresses();
    int serverPort;

public:
    Server(QObject *parent=0);
    void acceptFileTransfer(QString ID);

protected:
     void incomingConnection(int socketDescriptor);

private slots:
     void readIncomingData(QByteArray data);

public slots:
    void sendBroadcast(QByteArray datagram);
    void readBroadcast();

signals:
     void messageRecieved(QString message, QString username);
     void fileRecieved(QString name, qint64 size, QString ID, QString username);
     void dirRecieved(QDomNodeList fileList, QDomNodeList dirList, QString username);
     void udpDataRecieved(QHostAddress senderIP, QByteArray data);

};

#endif // SERVER_H
