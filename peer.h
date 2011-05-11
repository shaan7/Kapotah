/*
    This file is part of the Kapotah project.
    Copyright (C) 2010 Shantanu Tushar <jhahoneyk@gmail.com>
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


#ifndef KAPOTAH_PEER_H
#define KAPOTAH_PEER_H

#include <QString>
#include <QHostAddress>

namespace Kapotah
{

    /**
     * \brief Stores information about a peer
     *
     * The Peer class stores information such as name and IP address
     * of a peer. Can be extended in the future to include other
     * information
     */
    class Peer
    {

        public:
            Peer();
            Peer(const QString &name, const QHostAddress &ipAddress);

            /**
             * Returns the advertised name of the peer on the network
             *
             * @return  Peer's name
             */
            QString name() const;

            /**
             * Set the advertised name of the peer on the network
             * \warning For internal purposes only, do not use from the UI
             *
             * @param   name    Peer's name
             */
            void setName (const QString &name);

            /**
             * Returns the IP address of the peer's device
             *
             * @return  Peer's IP address
             */
            QHostAddress ipAddress() const;

            /**
             * Set the IP address of the peer
             * \warning For internal purposes only, do not use from the UI
             *
             * @param   ipAddress       IP address of peer
             */
            void setIpAddress (const QHostAddress &ipAddress);

            bool operator==(const Peer &peer);

        private:
            QString m_name;
            QHostAddress m_ipAddress;
    };
}

#endif // KAPOTAH_PEER_H
