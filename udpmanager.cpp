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


#include "udpmanager.h"
#include "xmlparser.h"
#include <qcoreevent.h>
#include <QNetworkInterface>
#include <QApplication>

using namespace Kapotah;

static const unsigned int s_broadcastPort = 45000;

template<> UdpManager *Kapotah::Singleton<UdpManager>::m_instance = 0;

UdpManager::UdpManager()
{
    updateAddresses();

    if (!m_broadcastSocket.bind (QHostAddress::Any, s_broadcastPort, QUdpSocket::ShareAddress
                            | QUdpSocket::ReuseAddressHint)) {
        qDebug() << "ERROR: Can't bind socket";
        QApplication::exit(1);
    }
    connect (&m_broadcastSocket, SIGNAL (readyRead()),
             this, SLOT (readBroadcast()));
}

UdpManager::~UdpManager()
{

}

void UdpManager::timerEvent (QTimerEvent* t)
{
    QObject::timerEvent (t);
}

void UdpManager::updateAddresses()
{
    m_broadcastAddresses.clear();
    m_ipAddresses.clear();
    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
        foreach (QNetworkAddressEntry entry, interface.addressEntries()) {
            QHostAddress broadcastAddress = entry.broadcast();

            if (broadcastAddress != QHostAddress::Null &&
                    entry.ip() != QHostAddress::LocalHost) {
                m_broadcastAddresses << broadcastAddress;
                m_ipAddresses << entry.ip();
            }
        }
    }
}

void UdpManager::readBroadcast()
{
    //Get the datagrams until socket in empty
    while (m_broadcastSocket.hasPendingDatagrams()) {
        QHostAddress senderIp;
        quint16 senderPort;
        QByteArray datagram;
        datagram.resize (m_broadcastSocket.pendingDatagramSize());

        if (m_broadcastSocket.readDatagram (datagram.data(), datagram.size(),
                                            &senderIp, &senderPort) == -1) {
            continue;
        }

        emit gotDatagram(datagram, senderIp, senderPort);
    }
}

void UdpManager::sendBroadcast (const QByteArray& datagram)
{
    bool validBroadcastAddresses = true;
    foreach (QHostAddress address, m_broadcastAddresses) {
        if (m_broadcastSocket.writeDatagram (datagram, address,
                                             s_broadcastPort) == -1)
            validBroadcastAddresses = false;
    }

    if (!validBroadcastAddresses)
        updateAddresses();
}

#include "udpmanager.moc"
