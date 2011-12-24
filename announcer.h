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


#ifndef KAPOTAH_ANNOUNCER_H
#define KAPOTAH_ANNOUNCER_H

#include "singleton.h"
#include "peer.h"

#include <QHostAddress>

namespace Kapotah
{

    /**
     *\brief This class processes user name announces, peer status, search, additional broadcasts
     *
     * The class can can get datagrams from the lower UdpManager layer and
     * process it to determine announces, peer statuses.
     * You can also use Announcer to add or remove additional broadcast addresses.
     * Can be also used to send search requests to the network and receive requests.
     */
    class Announcer : public Singleton<Announcer>
    {
            Q_OBJECT

        public:
            Announcer();
            virtual ~Announcer();

            /**
             * Returns the currently set username
             * 
             * @return the current username
             */
            QString username();

            /**
             * Set the current username
             * 
             * @param   username        the username to set
             */
            void setUserName (const QString &username);

        protected:
            virtual void timerEvent (QTimerEvent* t);

        private:
            int m_timerId;
            QString m_username;

        public slots:
            /**
             * Process datagram that was collected from UdpManager
             * 
             * @param   datagram        datagram received
             * @param   host    hostname
             * @param   port    port
             */
            void processDatagram (const QByteArray &datagram, const QHostAddress &host, quint16 port);

            /**
             * Called to send user's status to peer
             * 
             * @param   address         peer's address
             */
            void peerStatus(QHostAddress address);

            /**
             * Called when the UI sends a request to search for a file
             * 
             * @param   pattern the pattern to use while searching
             */
            void searchPeersForFile(const QString &pattern);

            /**
             * Call to add additional broadcast addresses
             * 
             * @param   broadcastAddress        the address to broadcast to
             */
            void addAdditionalBroadcastAddress(const QHostAddress &broadcastAddress);

            /**
             * Call to remove additional broadcast addresses
             * 
             * @param   broadcastAddress        the address to stop broadcasting to
             */
            void removeAdditionalBroadcastAddress(const QHostAddress &broadcastAddress);

        signals:
            /**
             * Emitted when a new peer has become online
             * 
             * @param   peer    details about the peer
             */
            void gotAnnounce (const Peer &peer);

            /**
             * Emitted when a peer's typing status has been received
             * 
             * @param   peer    details about the peer
             */
            void peerTyping (const QHostAddress &peer);

            /**
             * Emitted when a receiving progress has been received from the receipient
             *
             * @param   peer    details about the peer
             * @param   id      id of the Transfer
             * @param   bytesDone       bytes of transfer completed
             * @param   total   total size
             * @param speed     transfer speed
             */
            void gotProgress (const QHostAddress &peer, QString id, quint64 bytesDone, quint64 total, quint64 speed);

            /**
             * Emitted when a search request has been received on the network
             *
             * @param   pattern         the search request pattern
             * @param   host    details of the host who sent the request
             */
            void gotSearchRequest (const QString &pattern, const QHostAddress &host);
    };

}

#endif // KAPOTAH_ANNOUNCER_H
