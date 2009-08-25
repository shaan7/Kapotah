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
#include <QtNetwork>
#include <QStringList>
#include "peermanager.h"

static const qint32 BroadcastInterval = 1000;
static const unsigned broadcastPort = 45000;
static const int ageTimeout = 4;   //seconds

PeerManager::PeerManager()
         : QObject()
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

    updateAddresses();  //Updates the entry for broadcasts and IPs of this host
    serverPort = 0;

    broadcastSocket.bind(QHostAddress::Any, broadcastPort, QUdpSocket::ShareAddress
                         | QUdpSocket::ReuseAddressHint);
    connect(&broadcastSocket, SIGNAL(readyRead()),
            this, SLOT(readBroadcast()));

    broadcastTimer.setInterval(BroadcastInterval);
    connect(&broadcastTimer, SIGNAL(timeout()),
            this, SLOT(sendBroadcast()));
    connect(&broadcastTimer, SIGNAL(timeout()),
            this, SLOT(checkPeers()));
}

void PeerManager::startBroadcast()
{
    broadcastTimer.start();
}

PeerInfo PeerManager::peerInfo(QString name)
{
    return peers.value(name);
}

void PeerManager::sendBroadcast()
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

    QByteArray datagram(document.toByteArray());

    bool validBroadcastAddresses = true;
    foreach (QHostAddress address, broadcastAddresses) {
        if (broadcastSocket.writeDatagram(datagram, address,
                                          broadcastPort) == -1)
            validBroadcastAddresses = false;
    }

    if (!validBroadcastAddresses)
        updateAddresses();
}

void PeerManager::checkPeers()
{
    QList<PeerInfo>::Iterator iterator;
    QStringList toDelete;

    foreach (QString key, peers.keys()) {
        PeerInfo &peer = peers[key];
        peer.incrementAge();
        if (peer.age() > ageTimeout) {
            emit peerGone(peer.name());
            toDelete.append(peer.name());
        }
    }

    foreach (QString peer, toDelete) {
        peers.remove(peer);
    }
}

void PeerManager::readBroadcast()
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

        //Parse the datagram as XML
        QDomDocument document;
        document.setContent(datagram, false, 0, 0, 0);
        QDomElement documentElement = document.documentElement();
        QDomNode node = documentElement.firstChild();
        QDomElement action = node.toElement();

        //If data is type announce
        if (action.attribute("type") == "announce") {
            QDomElement announce = action.firstChild().toElement();
            PeerInfo tempPeer(announce.attribute("senderName", "unknown"),senderIp);

            if (!peers.contains(tempPeer.name())) {
                tempPeer.setAge(0);
                peers.insert(tempPeer.name(),tempPeer);
                emit newPeer(tempPeer.name());
            }
            else {
                peers[tempPeer.name()].setAge(0);
            }
        }
    }
}

void PeerManager::updateAddresses()
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
