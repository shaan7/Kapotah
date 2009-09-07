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

#include "dirsenderthread.h"
#include <QDir>
#include <QTime>
#include <QDomDocument>
#include <QCryptographicHash>

DirSenderThread::DirSenderThread(Pointers *ptr, QString dirToSend, PeerInfo destinationPeer, QObject *parent)
    : QThread(parent), manager(ptr->manager), fserver(ptr->fserver), dirPath(dirToSend), peer(destinationPeer)
{
}

void DirSenderThread::run()
{
    prepareItemsList(dirPath);

    QDir parentDir(dirPath);
    parentDir.cdUp();

    QDomDocument document;

    QDomElement doc = document.createElement("document");
    QDomElement action = document.createElement("action");
    action.setAttribute( "type", "dir" );
    action.setAttribute("senderName", manager->username());
    QDomElement files = document.createElement("files");
    QDomElement dirs = document.createElement("dirs");

    foreach (QString temp, fileList) {
        QFileInfo fileInfo(temp);
        QDomElement file = document.createElement("file");
        file.setAttribute("size", fileInfo.size());
        file.setAttribute("path", parentDir.relativeFilePath(temp));

        QString ID = QString(QCryptographicHash::hash(temp.toUtf8(), QCryptographicHash::Md5).toHex());
        file.setAttribute("ID", ID);

        if (!fserver->idExists(ID))
            fserver->addFile(ID, temp);

        files.appendChild(file);
    }

    foreach (QString temp, dirList) {
        QDomElement dir = document.createElement("dir");
        dir.setAttribute("path", parentDir.relativeFilePath(temp));
        dirs.appendChild(dir);
    }

    document.appendChild(doc);
    doc.appendChild(action);
    action.appendChild(files);
    action.appendChild(dirs);

    QTcpSocket socket;
    socket.abort();
    socket.connectToHost(peer.ipAddress(), 9876);
    socket.waitForConnected();

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

    deleteLater();
}

void DirSenderThread::prepareItemsList(QString path)
{
    dirList << path;
    QDir dir(path);

    if (dir.count()==0)
        return;

    foreach (QString file, dir.entryList(QStringList("*"), QDir::Files)) {
        fileList << dir.absoluteFilePath(file);
    }

    foreach (QString tdir, dir.entryList(QStringList("*"), QDir::Dirs | QDir::NoDotAndDotDot)) {
        dir.cd(tdir);
        prepareItemsList(dir.absolutePath());
        dir.cdUp();
    }
}

DirSenderThread::~DirSenderThread()
{
    wait();
    qDebug() << "DirSenderThread END";
}
