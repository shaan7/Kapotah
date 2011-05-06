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

void FileServerThread::run()    //TODO: use mutexes
{
    QString filename;
    QString ID;
    QFile file;
    QTcpSocket socket;
    socket.setSocketDescriptor (m_descriptor);

    while (!m_doQuit) {
        m_status = Waiting;
        while (!socket.bytesAvailable() && !m_doQuit) {
            socket.waitForReadyRead();
        }
        if (m_doQuit)
            break;

        QString data (socket.readAll());

        if (!Kapotah::TransferManager::instance()->pathForId (data).isEmpty()) {
            m_status = PreparingToSend;
            ID = data;
            filename = Kapotah::TransferManager::instance()->pathForId (data);

            file.setFileName (filename);

            if (!file.open (QIODevice::ReadOnly)) {
                m_status = ErrorFileNotFound;
                break;
            }

            socket.write ("OK");
            socket.waitForBytesWritten();
            emit startedTransfer (ID);
            m_status = Sending;

            while (!file.atEnd() && !m_doQuit) {
                if (socket.state() != QTcpSocket::ConnectedState) {
                    emit finishedTransfer (ID);
                    break;
                }

                socket.write (file.read (s_bytesPerBlock));
                socket.waitForBytesWritten();

                while (socket.bytesToWrite())
                    socket.flush();

                emit transferProgress (ID, file.pos() / file.size() *100);
            }

            file.close();

            if (m_doQuit) {
                emit canceledTransfer(ID);
                socket.disconnectFromHost();
            } else {
                emit finishedTransfer (ID);
            }

            socket.waitForDisconnected ();
            break;
        } else {
            m_status = ErrorIDNotFound;
            emit transferNotFound(ID);
            break;
        }

        deleteLater();
    }
}

FileServerThread::Status FileServerThread::status() const
{
    return m_status;
}

#include "fileserverthread.moc"
