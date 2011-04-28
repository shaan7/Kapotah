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


#include "peermanager.h"

using namespace Kapotah;

template<> PeerManager *Kapotah::Singleton<PeerManager>::m_instance = 0;

PeerManager::PeerManager() : m_peersModel(new PeersModel(this))
{
    connect(m_peersModel, SIGNAL(rowsInserted(const QModelIndex&, int, int)),
            SLOT(peerAddedInModel(QModelIndex,int,int)));
    connect(m_peersModel, SIGNAL(rowsRemoved(const QModelIndex&, int, int)),
            SLOT(peerRemovedInModel(QModelIndex,int,int)));
}

PeerManager::~PeerManager()
{

}

PeersModel* PeerManager::peersModel()
{
    return m_peersModel;
}

void PeerManager::peerAddedInModel (const QModelIndex& index, int start, int finish)
{
    emit peerAdded(m_peersModel->index(start));
}

void PeerManager::peerRemovedInModel (const QModelIndex& index, int start, int finish)
{
    emit peerAdded(m_peersModel->index(start));
}

#include "peermanager.moc"
