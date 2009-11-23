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

const quint64 bytesPerBlock = Q_UINT64_C(100000);   //number of bytes to transfer in one block

FileRecieverThread::FileRecieverThread(Pointers *ptr, QString fileID, quint64 fileSize, QHostAddress senderIP, QString outputFilename, QObject *parent)
    : QThread(parent), ID(fileID), peerIP(senderIP), manager(ptr->manager), filename(outputFilename), size(fileSize)
{
    readyToRecieve = false;
    startedRecieving = false;
    bytesCopied = 0;
    doQuit = false;
}

void FileRecieverThread::stopTransfer()
{
    doQuit = true;
}

void FileRecieverThread::run()
{
    if (size==0)
        return;         //TODO: better to create an empty file
    QTcpSocket socket;
    socket.connectToHost(peerIP, 9877);
    socket.waitForConnected();

    socket.write(ID.toUtf8());  //ID
    socket.waitForBytesWritten();

    while (!doQuit) {

        while (socket.bytesAvailable()==0) {
            socket.waitForReadyRead();
        }

        if (readyToRecieve) {
            quint64 bytesWritten = file.write(socket.readAll());
            bytesCopied = file.pos();

            emit progress(ID, peerIP, filename, size, bytesCopied);

            if (bytesCopied >= size) {
                qDebug() << "DONE " << filename;
                emit done(ID);
                file.close();
                socket.disconnectFromHost();

                if (socket.state()!=QTcpSocket::UnconnectedState)
                    socket.waitForDisconnected();
                break;
            }
        }
        else {
            QString data(socket.read(2));
            if (data == "OK") {
                readyToRecieve = true;
                file.setFileName(filename);
                if (!file.open(QIODevice::WriteOnly)) {
                    qDebug() << "ERROR OPENING FILE " << file.errorString();
                    break;
                }
            }
            else {
                qDebug() << ID << " not found on server: " << data;
                break;
            }
        }
    }
    deleteLater();
}

FileRecieverThread::~FileRecieverThread()
{
    doQuit = true;
    file.close();
    wait();
    qDebug() << "FileRecieverThread END";
}
