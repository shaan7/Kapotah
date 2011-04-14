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


#include "peer.h"

using namespace Kapotah;

Peer::Peer()
{

}

Peer::Peer (const QString& name, const QHostAddress& ipAddress)
    : m_name(name), m_ipAddress(ipAddress)
{

}

Peer::~Peer()
{

}

QHostAddress Peer::ipAddress() const
{
    return m_ipAddress;
}

void Peer::setIpAddress (const QHostAddress& ipAddress)
{
    m_ipAddress = ipAddress;
}

QString Peer::name() const
{
    return m_name;
}

void Peer::setName (const QString& name)
{
    m_name = name;
}

bool Peer::operator== (const Peer& peer)
{
    return ((m_ipAddress == peer.ipAddress()) && (m_name == peer.name()));
}
