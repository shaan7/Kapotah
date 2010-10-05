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

#include <QDomDocument>
#include <QStringList>
#include <QProcess>
#include "peermanager.h"

static const qint32 BroadcastInterval = 1000;
static const unsigned broadcastPort = 45000;
static const int ageTimeout = 4;   //seconds

PeerManager::PeerManager(Pointers *ptr)
         : QObject(), server(ptr->server)
{
    QStringList envVariables;   //Used to get one of the below infos about the client operating enivronment
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";

    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, envVariables) {
        int index = environment.indexOf(QRegExp(string));
        if (index != -1) {
            QStringList stringList = environment.at(index).split("=");
            if (stringList.size() == 2) {
                m_username = stringList.at(1).toUtf8();
                break;
            }
        }
    }

    if (m_username.isEmpty())
        m_username = "unknown";
}

bool PeerManager::contains(QString ipAddress)
{
    return peers.contains(ipAddress);
}

void PeerManager::startBroadcast()
{
    broadcastTimer.setInterval(BroadcastInterval);
    connect(server, SIGNAL(udpDataRecieved(QHostAddress,QByteArray)), this, SLOT(parseUdpDatagram(QHostAddress,QByteArray)));
    connect(&broadcastTimer, SIGNAL(timeout()), this, SLOT(sendAnnounce()));
    connect(&broadcastTimer, SIGNAL(timeout()), this, SLOT(checkPeers()));

    broadcastTimer.start();
}

PeerInfo PeerManager::peerInfo(QString ipAddress)
{
    return peers.value(ipAddress);
}

void PeerManager::checkPeers()
{
    QList<PeerInfo>::Iterator iterator;
    QStringList toDelete;

    foreach (QString key, peers.keys()) {
        PeerInfo &peer = peers[key];
        peer.incrementAge();
        if (peer.age() > ageTimeout) {
            emit peerGone(peer.ipAddress());
            toDelete.append(peer.ipAddress().toString());
        }
    }

    foreach (QString peer, toDelete) {
        peers.remove(peer);
    }
}

void PeerManager::sendAnnounce()
{
    QDomDocument document;

    QDomElement doc = document.createElement("document");
    QDomElement action = document.createElement("action");
    action.setAttribute( "type", "announce" );
    QDomElement announce = document.createElement("announce");
    announce.setAttribute("senderName", m_username);

    document.appendChild(doc);
    doc.appendChild(action);
    action.appendChild(announce);

    server->sendBroadcast(document.toByteArray());
}

void PeerManager::parseUdpDatagram(QHostAddress senderIP, QByteArray datagram)
{
    QDomDocument document;
    document.setContent(datagram, false, 0, 0, 0);
    QDomElement documentElement = document.documentElement();
    QDomNode node = documentElement.firstChild();
    QDomElement action = node.toElement();

    //If data is type announce
    if (action.attribute("type") == "announce") {
        QDomElement announce = action.firstChild().toElement();
        PeerInfo tempPeer(announce.attribute("senderName", "unknown"),senderIP);

        if (!peers.contains(tempPeer.ipAddress().toString())) {
            tempPeer.setAge(0);
            peers.insert(tempPeer.ipAddress().toString(),tempPeer);
            emit newPeer(tempPeer.ipAddress());
        }
        else {
            peers[tempPeer.ipAddress().toString()].setAge(0);
        }
    }
}
