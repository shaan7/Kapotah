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

#include <QFile>
#include <QByteArray>
#include "filerecieverthread.h"
#include "peerinfo.h"

const qint64 bytesPerBlock = Q_INT64_C(100000);   //number of bytes to transfer in one block

FileRecieverThread::FileRecieverThread(PeerManager *peerManager, QString fileID, qint64 fileSize, QString sendingPeer, QString outputFilename, QObject *parent)
    : QThread(parent)
{
    ID = fileID;
    peer = sendingPeer;
    manager = peerManager;
    readyToRecieve = false;
    startedRecieving = false;
    filename = outputFilename;
    size = fileSize;
    bytesCopied = 0;
    quit = false;
}

void FileRecieverThread::run()
{
    QTcpSocket socket;
    socket.connectToHost(manager->peerInfo(peer).ipAddress(), 9877);
    socket.waitForConnected();

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << ID.toUtf8();

    socket.write(data);  //ID
    socket.waitForBytesWritten();

    while (!quit) {
        socket.waitForReadyRead();

        QByteArray recv;
        QDataStream in(&socket);
        in.setVersion(QDataStream::Qt_4_5);
        in >> recv;

        if (readyToRecieve) {
            qint64 bytesWritten = file.write(recv);
            bytesCopied = file.pos();
            //qDebug() << "WROTE " << bytesWritten;

            emit progress(bytesCopied);

            if (bytesCopied < size) {
                QByteArray data;
                QDataStream out(&data, QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_5);
                out << (QString("SENDMORE")+QString::number(bytesCopied)).toUtf8();

                socket.write(data);  //SENDMOREbytesCopied
                socket.waitForBytesWritten();
            }
            else {
                emit done();
                file.close();
                socket.disconnectFromHost();
                break;
            }
        }
        else {
            if (QString(recv) == "OK") {
                readyToRecieve = true;
                file.setFileName(filename);
                if (!file.open(QIODevice::WriteOnly)) {
                    qDebug() << "ERROR OPENING FILE";
                    return;
                }
                QByteArray data;
                QDataStream out(&data, QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_5);
                out << QString("SENDMORE0").toUtf8();

                socket.write(data);  //SENDMOREbytesCopied
                socket.waitForBytesWritten();
            }
        }
    }
}

FileRecieverThread::~FileRecieverThread()
{
    quit = true;
    wait();
}
