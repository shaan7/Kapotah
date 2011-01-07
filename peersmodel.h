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

    class PeersModel : public QAbstractListModel
    {
            Q_OBJECT

        public:
            enum Roles { ipAddressRole = Qt::UserRole };
            virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
            virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;
            PeersModel (QObject* parent = 0);
            virtual ~PeersModel();

        private:
            QHash<QHostAddress, Peer> m_peers;
            QList<QHostAddress> m_peerList;

        public slots:
            void addNewPeer (Peer peer);
    };

}

#endif // KAPOTAH_PEERSMODEL_H
