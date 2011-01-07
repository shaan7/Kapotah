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


#include "messagedispatcherthread.h"

#include <QTcpSocket>

MessageDispatcherThread::MessageDispatcherThread (int socketDescriptor, QObject* parent) :
    m_descriptor(socketDescriptor), QThread (parent), m_doQuit(false)
{
}


MessageDispatcherThread::~MessageDispatcherThread()
{
    m_doQuit = true;
    wait();
}

void MessageDispatcherThread::run()
{
    QTcpSocket socket;
    socket.setSocketDescriptor(m_descriptor);
    quint64 dataSize = 0;

    while(!m_doQuit)
    {
        socket.waitForReadyRead();

        if (dataSize == 0) {
            QDataStream stream(&socket);
            stream.setVersion(QDataStream::Qt_4_6);

            if (socket.bytesAvailable() < sizeof(quint64))
                continue;

            stream >> dataSize;
        }

        if (socket.bytesAvailable() < dataSize)
            continue;

        emit gotMessage(socket.readAll(), socket.peerAddress());
        break;
    }

    deleteLater();
}

#include "messagedispatcherthread.moc"

