/*
    This file is part of the Kapotah project.
    Copyright (C) 2009 Shantanu Tushar <jhahoneyk@gmail.com>

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


#include "peersmodel.h"
#include "broadcastmanager.h"

using namespace Kapotah;

QVariant PeersModel::data (const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        return m_peers[m_peerList.at (index.row()) ].name();
    }

    else if (role == ipAddressRole)
    {
        return m_peers[m_peerList.at (index.row()) ].ipAddress().toString();
    }

    return QVariant();
}

int PeersModel::rowCount (const QModelIndex& parent) const
{
    return m_peerList.count();
}

PeersModel::PeersModel (QObject* parent) : QAbstractListModel (parent)
{
    connect (BroadcastManager::instance(), SIGNAL (gotAnnounce (Peer)), SLOT (addNewPeer (Peer)));
}


PeersModel::~PeersModel()
{

}

void PeersModel::addNewPeer (Peer peer)
{
    if (m_peers.contains (peer.ipAddress()))
        return;

    int row = rowCount();

    beginInsertRows (QModelIndex(), row, row);

    qDebug() << "Adding " << peer.name() << " at " << peer.ipAddress() << " as " << row;

    m_peerList.append (peer.ipAddress());

    m_peers[peer.ipAddress() ] = peer;

    endInsertRows();
}

#include "peersmodel.moc"
