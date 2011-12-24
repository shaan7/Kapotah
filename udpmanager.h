/*
    This file is part of the Kapotah project.
    Copyright (C) 2010 Shantanu Tushar <shaan7in@gmail.com>
    Copyright (C) 2010 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

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


#ifndef KAPOTAH_UDPMANAGER_H
#define KAPOTAH_UDPMANAGER_H

#include "singleton.h"
#include "peer.h"

#include <QObject>
#include <QUdpSocket>

namespace Kapotah
{

    /**
     * \brief Handles UDP datagrams
     *
     * This Singleton class handles low level UDP functions such as
     * sending, broadcasts and unicasts, and receiving datagrams.
     * Other library classes can connect to this to get the data.
     */
    class UdpManager : public Singleton<UdpManager>
    {
            Q_OBJECT

        public:
            UdpManager();

            /**
             * Update brodcast addresses, localhost addreses.
             * Typically should be called after the user indicates
             * a change in network configurations
             */
            void updateAddresses();

            /**
             * Used to check if an IP belongs to the local device
             * @param   ip      IP address to be checked
             * @return  true if IP is locahost, false otherwise
             */
            bool isLocalHostIp(const QHostAddress &ip);

        protected:
            virtual void timerEvent (QTimerEvent*);

        private:
            QList<QHostAddress> m_broadcastAddresses;
            QList<QHostAddress> m_additionalBroadcastAddresses;
            QList<QHostAddress> m_ipAddresses;
            QUdpSocket m_broadcastSocket;
            int m_timerId;

        private slots:
            void readBroadcast();

        public slots:
            /**
             * Send a brodcast on the network, and additional addresses
             *
             * @param   datagram        the datagram to broadcast
             */
            void sendBroadcast (const QByteArray &datagram);

            /**
             * Send a datagram to a particular host
             *
             * @param   datagram        the datagram to send
             * @param   host    host to which the datagram should be sent
             */
            void sendDatagram (const QByteArray &datagram, const QHostAddress &host);

            /**
             * Library call to add additional broadcast addresses
             *
             * @param   broadcastAddress        the address to broadcast to
             */
            void addBroadcastAddress (const QHostAddress &broadcastAddress);

            /**
             * Library call to remove additional broadcast addresses
             *
             * @param   broadcastAddress        the address to stop broadcasting to
             */
            void removeBroadcastAddress (const QHostAddress &broadcastAddress);

        signals:
            /**
             * Emitted when a new datagram is received
             * \note Datagram maybe broadcast or unicast
             *
             * @param   datagram        the datagram received
             * @param   host    host which sent the datagram
             * @param   port    sender port (generally not useful)
             */
            void gotDatagram (const QByteArray &datagram, const QHostAddress &host, quint16 port);
    };

}

#endif // KAPOTAH_UDPMANAGER_H
