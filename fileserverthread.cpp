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

#include <QDataStream>
#include "fileserverthread.h"
#include "fileserver.h"

const qint64 bytesPerBlock = Q_INT64_C(100000);   //number of bytes to transfer in one block

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

        QByteArray recv;
        QDataStream stream(&socket);
        stream.setVersion(QDataStream::Qt_4_5);
        stream >> recv;
        QString response(recv);

        if (response.contains("SENDMORE")) {
            qint64 requestedPos = response.right(response.length()-8).toLongLong();
            if (file.pos()!=requestedPos) {
                qDebug() << "MISMATCH " << requestedPos << " INSTEAD OF " << file.pos();
                file.seek(requestedPos);
            }

            char a[bytesPerBlock];
            QByteArray data;
            QDataStream out(&data, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_5);
            qDebug() << "READ " << file.read(a,bytesPerBlock);
            out << a;
            qDebug() << "DATA " << data.length();

            socket.write(data);
            socket.waitForBytesWritten();
        }

        else if (dynamic_cast<FileServer*>(parent())->getFileList()->contains(response)) {
            ID = response;
            filename = dynamic_cast<FileServer*>(parent())->getFileList()->value(ID);

            file.setFileName(filename);
            if (!file.open(QIODevice::ReadOnly))
                return;

            QByteArray data;
            QDataStream out(&data, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_5);
            out << "OK";

            socket.write(data);
            socket.waitForBytesWritten();
        }
    }
}

FileServerThread::~FileServerThread()
{
    quit = true;
    wait();
}
