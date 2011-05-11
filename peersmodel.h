/*
    This file is part of the Kapotah project.
    Copyright (C) 2010 Shantanu Tushar <jhahoneyk@gmail.com>

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


#ifndef KAPOTAH_PEERSMODEL_H
#define KAPOTAH_PEERSMODEL_H

#include "peer.h"

#include <QAbstractListModel>

namespace Kapotah
{

    /**
     * \brief Model containing peer list data
     *
     * This model contains data about online peers on the network
     * \note Avoid accessing this directly, user PeerManager
     */
    class PeersModel : public QAbstractListModel
    {
            Q_OBJECT

        public:
            enum Role { ipAddressRole = Qt::UserRole };
            PeersModel (QObject* parent = 0);
            /**
             * Return data for the given peer
             *
             * @param   index   model index corresponding to the peer
             * @param   role    the Role of the data requested
             */
            virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
            virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;

            /**
             * Returns the peer's name according to IP address
             *
             * @param   address         IP address of peer
             */
            QString peerNameForIp(const QHostAddress &address);

        private:
            QList<QHostAddress> m_peerAddresses;
            QHash<QHostAddress, Peer> m_peers;
            QHash<QHostAddress, int> m_ages;

        private slots:
            void checkStatus();
            void addNewPeer (Peer peer);

        protected:
            virtual void timerEvent (QTimerEvent* event);
    };

}

#endif // KAPOTAH_PEERSMODEL_H
