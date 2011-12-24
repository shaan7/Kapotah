/*
    This file is part of the Kapotah project.
    Copyright (C) 2010 Shantanu Tushar <shaan7in@gmail.com>

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


#ifndef KAPOTAH_PEERMANAGER_H
#define KAPOTAH_PEERMANAGER_H

#include "singleton.h"
#include "peersmodel.h"

namespace Kapotah
{

    /**
     * \brief Singleton which serves as central management of peer information
     *
     * This class stores the list the client has of the peers on the network.
     * The list is updated from the network time to time, and dead peers are
     * removed as required.
     * The class also provides convenience functions to get name, ip etc for a
     * peer according to model indices
     */
    class PeerManager : public Singleton<PeerManager>
    {
            Q_OBJECT
        public:
            PeerManager();

            /**
             * Return a pointer to the PeersModel instance
             *
             * @return pointer to the PeersModel instance
             */
            PeersModel *peersModel();
            QHostAddress ipFromIndex(const QModelIndex &index);
            QString nameFromIndex(const QModelIndex &index);
            QString nameFromIp(const QHostAddress &address);

        private:
            PeersModel *m_peersModel;

        private slots:
            void peerAddedInModel(const QModelIndex &index, int start, int finish);
            void peerRemovedInModel(const QModelIndex &index, int start, int finish);

        signals:
            /**
             * Emitted when a new peer has been added to the model
             *
             * @param   peerAddress     address of the peer
             */
            void peerAdded(const QHostAddress &peerAddress);

            /**
             * Emitted when a new peer has been removed from the model
             *
             * @param   peerAddress     address of the peer
             */
            void peerRemoved(const QHostAddress &peerAddress);
    };

}

#endif // KAPOTAH_PEERMANAGER_H

