/***************************************************************************
 *   Copyright 2009 Shantanu Tushar <jhahoneyk@gmail.com>                  *
 *   Copyright 2009 Sudhendu Kumar <sudhendu_roy@yahoo.co.in>                *
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

#include <QDataStream>
#include "fileserverthread.h"
#include "fileserver.h"

const quint64 bytesPerBlock = Q_UINT64_C(50000);   //number of bytes to transfer in one block

FileServerThread::FileServerThread(int descriptor, QObject *parent) : QThread(parent)
{
    desc = descriptor;
    quit = false;
}

void FileServerThread::run()
{
    QTcpSocket socket;
    socket.setSocketDescriptor(desc);

    while(!quit)
    {
        socket.waitForReadyRead();

        QString data(socket.readAll());
        if (dynamic_cast<FileServer*>(parent())->idExists(data)) {  //dynamic_cast<FileServer*>(parent()) is actually the fileserver
            ID = data;
            filename = dynamic_cast<FileServer*>(parent())->getFileName(ID);

            file.setFileName(filename);
            if (!file.open(QIODevice::ReadOnly))
                break;

            socket.write("OK");
            socket.waitForBytesWritten();

            emit startedTransfer(ID, filename);
            while (!file.atEnd()) {
                if (socket.state() != QTcpSocket::ConnectedState) {
                    emit finishedTransfer(ID, filename);
                    break;
                }
                socket.write(file.read(bytesPerBlock));
                socket.waitForBytesWritten();
                while (socket.bytesToWrite())
                    socket.flush();
                qDebug() << "EMITTING " << file.pos();
                emit transferProgress(ID, file.pos());
            }
            file.close();
            socket.waitForDisconnected(-1);
            emit finishedTransfer(ID, filename);
            break;
        }
        else
            break;
    }
    deleteLater();
}

FileServerThread::~FileServerThread()
{
    quit = true;
    file.close();
    wait();
    qDebug() << "FileServerThread END";
}
