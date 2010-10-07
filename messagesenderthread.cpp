/*
    This file is part of the Kapotah project.
    Copyright (C) 2009 Shantanu Tushar <jhahoneyk@gmail.com>
    Copyright (C) 2009 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

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

#include "messagesenderthread.h"

#include <QTcpSocket>

static const int s_messageServerPort = 45001;

MessageSenderThread::MessageSenderThread (QString message, QHostAddress peerAddress, QObject* parent)
    : m_message(message), m_peerAddress(peerAddress), QThread (parent)
{

}

MessageSenderThread::~MessageSenderThread()
{
    wait();
}

void MessageSenderThread::run()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_6);
    stream << (quint64) m_message.toUtf8().size();
    data.append(m_message.toUtf8());

    QTcpSocket socket;
    socket.abort();
    socket.connectToHost(m_peerAddress, s_messageServerPort);
    socket.waitForConnected();
    socket.write(data);
    socket.waitForBytesWritten();
    socket.disconnectFromHost();
    if (socket.state()!=QTcpSocket::UnconnectedState)
        socket.waitForDisconnected();

    deleteLater();
}

#include "messagesenderthread.moc"
