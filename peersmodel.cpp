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


#include "peersmodel.h"
#include "announcer.h"

using namespace Kapotah;

static const int agingInterval = 1000;          //milliseconds after which to age the peers
static const int peerAnnounceTimeout = 4;    //seconds after which a peer can be assumed dead

QVariant PeersModel::data (const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        return m_peers[m_peers.keys().at (index.row()) ].name();
    }

    else if (role == ipAddressRole)
    {
        return m_peers[m_peers.keys().at (index.row()) ].ipAddress().toString();
    }

    return QVariant();
}

int PeersModel::rowCount (const QModelIndex& parent) const
{
    return m_peers.keys().count();
}

PeersModel::PeersModel (QObject* parent) : QAbstractListModel (parent)
{
    connect (Announcer::instance(), SIGNAL (gotAnnounce (Peer)), SLOT (addNewPeer (Peer)));
    startTimer(1000);
}

PeersModel::~PeersModel()
{

}

void PeersModel::addNewPeer (Peer peer)
{
    if (m_peers.contains (peer.ipAddress())) {
        m_ages[peer.ipAddress()] = 0;
        if (m_peers[peer.ipAddress()].name() != peer.name()) {
            Peer &existingPeer = m_peers[peer.ipAddress()];
            existingPeer.setName(peer.name());
            emit dataChanged(createIndex(m_peers.keys().indexOf(peer.ipAddress()), 0),
                        createIndex(m_peers.keys().indexOf(peer.ipAddress()), 0));
        }
        return;
    }

    int row = rowCount();

    beginInsertRows (QModelIndex(), row, row);

    qDebug() << "Adding " << peer.name() << " at " << peer.ipAddress() << " as " << row;

    m_peers[peer.ipAddress() ] = peer;
    m_ages[peer.ipAddress()] = 0;

    endInsertRows();
}

void PeersModel::checkStatus()
{
    QList<QHostAddress> addressesToRemove;

    foreach (QHostAddress host, m_peers.keys()) {
        m_ages[host]++;
        if (m_ages[host] > peerAnnounceTimeout) {
            addressesToRemove.append(host);
        }
    }

    foreach (QHostAddress address, addressesToRemove) {
        int row = m_peers.keys().indexOf(address);
        beginRemoveRows(QModelIndex(), row, row);
        m_peers.remove(address);
        endRemoveRows();
    }
}

void PeersModel::timerEvent(QTimerEvent* event)
{
    checkStatus();
}

QString PeersModel::peerNameForIp(const QHostAddress& address)
{
    return m_peers[address].name();
}

#include "peersmodel.moc"
