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

#include <QFileInfo>
#include <QDomDocument>
#include <QTime>
#include "filesenderthread.h"

FileSenderThread::FileSenderThread(Pointers *ptr, QString fileToSend, PeerInfo destinationPeer, QObject *parent)
    : QThread(parent), manager(ptr->manager), fserver(ptr->fserver), filename(fileToSend), destination(destinationPeer)
{
    qsrand(1000);
}

void FileSenderThread::run()
{
    QTcpSocket socket;
    socket.abort();
    socket.connectToHost(destination.ipAddress(), 9876);
    socket.waitForConnected();

    //Get info about the file
    QFileInfo info(filename);
    QDomDocument document;

    QDomElement doc = document.createElement("document");
    QDomElement action = document.createElement("action");
    action.setAttribute( "type", "file" );
    action.setAttribute("senderName", manager->username());
    QDomElement file = document.createElement("file");
    file.setAttribute("fileName", info.fileName());
    file.setAttribute("size", QString::number(info.size()));
    ID = QTime::currentTime().toString() + "::" + QString::number(qrand());
    file.setAttribute("ID", ID);

    if (!fserver->idExists(ID)) {
        fserver->addFile(ID, filename);
    }

    document.appendChild(doc);
    doc.appendChild(action);
    action.appendChild(file);

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_5);
    stream << (quint32)document.toByteArray().size();
    data.append(document.toByteArray());

    socket.write(data);
    socket.waitForBytesWritten();
    socket.disconnectFromHost();

    if (socket.state()!=QTcpSocket::UnconnectedState)
        socket.waitForDisconnected();
}
