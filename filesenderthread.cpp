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

FileSenderThread::FileSenderThread(PeerManager *peermanager, FileServer *fileServer, QString fileToSend, PeerInfo destinationPeer)
{
    manager = peermanager;
    fserver = fileServer;
    filename = fileToSend;
    destination = destinationPeer;

    qsrand(1000);
}

void FileSenderThread::run()
{
    socket.abort();
    socket.connectToHost(destination.ipAddress(), 9876);
    connect(&socket, SIGNAL(connected()), this, SLOT(sendFileInformation()));
}

void FileSenderThread::sendFileInformation()
{
    //Get info about the file
    QFileInfo info(filename);
    QDomDocument document;

    QDomElement doc = document.createElement("document");
    QDomElement action = document.createElement("action");
    action.setAttribute( "type", "file" );
    QDomElement file = document.createElement("file");
    file.setAttribute("senderName", manager->username());
    file.setAttribute("fileName", info.fileName());
    file.setAttribute("size", QString::number(info.size()));
    ID = QTime::currentTime().toString() + "::" + QString::number(qrand());
    file.setAttribute("ID", ID);

    if (!fserver->getFileList()->contains(ID)) {
        fserver->addFile(ID, filename);
    }

    document.appendChild(doc);
    doc.appendChild(action);
    action.appendChild(file);

    socket.write(document.toByteArray());
    socket.disconnectFromHost();
    socket.waitForDisconnected();
    deleteLater();
}
