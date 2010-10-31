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

Transfer::Transfer (Transfer::TransferType type, QList< TransferFile > files, QHostAddress peer, QObject* parent)
    : QObject (parent), m_type(type), m_files(files), m_peerAddress(peer), m_thread (0)
{
}

Transfer::~Transfer()
{

}

void Transfer::start()
{
    m_filesIterator = m_files.begin();
    startNextFile();
}

void Transfer::startNextFile()
{
    if (m_type == Incoming) {
        m_thread = new IncomingTransferThread (m_peerAddress, m_filesIterator->id, m_filesIterator->path, m_filesIterator->size, this);
    } else if (m_type = Outgoing) {
        m_thread = new OutgoingTransferThread (m_peerAddress, m_files, this);
    }

    connect (m_thread, SIGNAL (done (QString)), SLOT (currentFileFinished()));
    m_thread->start();
}

void Transfer::currentFileFinished()
{
    m_thread->wait();
    ++m_filesIterator;

    if (m_filesIterator == m_files.end()) {
        emit done();
    } else {
        startNextFile();
    }
}

void Transfer::reportProgress (quint64 done, quint64 size)
{
    emit (done, size);
}


#include "transfer.moc"
