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

#include "incomingtransferthread.h"

#include <QFile>
#include <QTcpSocket>

IncomingTransferThread::IncomingTransferThread (QHostAddress ip, QString id, QString filename, quint64 size,
                                                QObject* parent): TransferThread (ip, id, filename, size,
                                                                                  parent)
{
    doQuit = false;
}

IncomingTransferThread::~IncomingTransferThread()
{

}

void IncomingTransferThread::stopTransfer()
{
    doQuit = true;
}

void IncomingTransferThread::run()
{
    bool readyToReceive = false;
    quint64 bytesCopied;

    QFile file;

    if (m_size == 0) {  //Create an empty file
        file.setFileName (m_filename);
        file.open (QIODevice::WriteOnly);
        emit done (m_id);
        file.close();
        return;
    }

    QTcpSocket socket;

    socket.connectToHost (m_ip, 9877);
    socket.waitForConnected();

    socket.write (m_id.toUtf8()); //ID
    socket.waitForBytesWritten();

    while (!doQuit) {
        while (socket.bytesAvailable() == 0) {
            socket.waitForReadyRead();
        }

        if (readyToReceive) {
            file.write (socket.readAll());
            bytesCopied = file.pos();

            emit progress (m_id, bytesCopied, m_size);

            if (bytesCopied >= m_size) {
                emit done (m_id);
                file.close();
                socket.disconnectFromHost();

                if (socket.state() != QTcpSocket::UnconnectedState)
                    socket.waitForDisconnected();
                break;
            }
        } else {
            QString data (socket.read (2));

            if (data == "OK") {
                readyToReceive = true;
                file.setFileName (m_filename);

                if (!file.open (QIODevice::WriteOnly)) {
                    break;
                }
            } else {
                qDebug() << m_id << " not found on server: " << data;
                break;
            }
        }
    }

    deleteLater();
}

#include "incomingtransferthread.moc"
