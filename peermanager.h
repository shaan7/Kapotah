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

#ifndef PEERMANAGER_H
#define PEERMANAGER_H

#include <QObject>
#include <QTimer>
#include "peerinfo.h"
#include "server.h"

class PeerManager : public QObject
{
    Q_OBJECT

private:
    QString m_username;
    Server *server;
    QHash<QString, PeerInfo> peers;
    QTimer broadcastTimer;

public:
    PeerManager(Server *serverPtr);
    void startBroadcast();
    void setUsername(QString name)  {   m_username = name;    }
    QString username()  {   return m_username;  }
    PeerInfo peerInfo(QString name);
    bool contains(QString name);
signals:
    void newPeer(QString name);
    void peerGone(QString name);

public slots:
    void checkPeers();
    void parseUdpDatagram(QHostAddress senderIP, QByteArray datagram);
    void sendAnnounce();
};

#endif // PEERMANAGER_H
