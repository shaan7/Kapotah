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

#include "transfer.h"
#include "incomingtransferthread.h"
#include "outgoingtransferthread.h"

using namespace Kapotah;

Transfer::Transfer (QList< TransferFile > files, QHostAddress peer, QObject* parent)
        : QObject (parent), m_sizeDone (0), m_filesDone (0), m_files (files), m_peerAddress (peer),
        m_thread (0), m_state (Waiting)
{

}

Transfer::~Transfer()
{

}

#include "transfer.moc"
