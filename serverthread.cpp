/***************************************************************************
 *   Copyright 2009 Shantanu Tushar <jhahoneyk@gmail.com>                  *
 *   Copyright 2009 Sudhendu Roy <sudhendu_roy@yahoo.co.in>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "serverthread.h"

ServerThread::ServerThread(int descriptor, QObject *parent) : QThread(parent), desc(descriptor)
{
    doQuit = false;
    dataSize = 0;
}

void ServerThread::run()
{
    QTcpSocket socket;
    socket.setSocketDescriptor(desc);

    while(!doQuit)
    {
        socket.waitForReadyRead();

        if (dataSize == 0) {
            QDataStream stream(&socket);
            stream.setVersion(QDataStream::Qt_4_5);

            if (socket.bytesAvailable() < sizeof(quint32))
                continue;

            stream >> dataSize;
        }

        if (socket.bytesAvailable() < dataSize)
            continue;

        emit dataReady(socket.readAll());
        return;
    }
}

ServerThread::~ServerThread()
{
    doQuit = true;
    wait();
}
