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

#include "fileserverthread.h"
#include "transfermanager.h"

#include <QTcpSocket>
#include <qfile.h>

static const quint64 s_bytesPerBlock = Q_UINT64_C (50000);  //number of bytes to transfer in one block

FileServerThread::FileServerThread (int socketDescriptor, QObject* parent)
        : QThread (parent), m_descriptor (socketDescriptor), m_doQuit(false)
{
}

FileServerThread::~FileServerThread()
{
    m_doQuit = true;
    wait();
}

void FileServerThread::run()
{
    QString filename;
    QString ID;
    QFile file;
    QTcpSocket socket;
    socket.setSocketDescriptor (m_descriptor);

    while (!m_doQuit) {
        while (socket.bytesAvailable() == 0) {
            socket.waitForReadyRead();
        }

        QString data (socket.readAll());

        if (!Kapotah::TransferManager::instance()->pathForId (data).isEmpty()) {
            ID = data;
            filename = Kapotah::TransferManager::instance()->pathForId (data);

            file.setFileName (filename);

            if (!file.open (QIODevice::ReadOnly))
                break;  //TODO: too quiet, let the system know the error

            socket.write ("OK");
            socket.waitForBytesWritten();
            emit startedTransfer (ID);

            while (!file.atEnd()) {
                if (socket.state() != QTcpSocket::ConnectedState) {
                    emit finishedTransfer (ID);
                    break;
                }

                socket.write (file.read (s_bytesPerBlock));
                socket.waitForBytesWritten();

                while (socket.bytesToWrite())
                    socket.flush();

                emit transferProgress (ID, file.pos() / file.size() *100);
                //qDebug() << "Sent " << file.pos() << " of " << file.size();
            }

            file.close();

            socket.waitForDisconnected (-1);
            emit finishedTransfer (ID);
            break;
        } else
            break;      //TODO: too quiet, let the system know the error
    }

    //deleteLater();    //FIXME: what to do?
}

#include "fileserverthread.moc"
