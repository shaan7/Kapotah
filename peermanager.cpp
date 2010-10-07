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


#include "peermanager.h"

using namespace Kapotah;

template<> PeerManager *Kapotah::Singleton<PeerManager>::m_instance = 0;

PeerManager::PeerManager() : m_peersModel(new PeersModel(this))
{

}

PeerManager::~PeerManager()
{

}

PeersModel* PeerManager::peersModel()
{
    return m_peersModel;
}
