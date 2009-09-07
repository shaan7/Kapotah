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

#include "dirrecieverthread.h"
#include <QDir>

DirRecieverThread::DirRecieverThread(Pointers *ptr, QString ID, QString dir, QDomNodeList files, QDomNodeList dirs, QString peername, QObject *parent)
    : QThread(parent), manager(ptr->manager), fileList(files), dirList(dirs), peerName(peername), dirName(dir), m_ptr(ptr), m_ID(ID)
{
    cancel = false;
}

void DirRecieverThread::run()
{
    QDir dir(dirName);
    for (int i=0;i<dirList.count();i++) {
        dir.mkdir(dirList.at(i).toElement().attribute("path"));
    }

    for (int i=0;i<fileList.count();i++) {
        if (cancel)
            break;
        fileReciever = new FileRecieverThread(m_ptr, fileList.at(i).toElement().attribute("ID"),
                                          fileList.at(i).toElement().attribute("size").toInt(), peerName,
                                          dir.absoluteFilePath(fileList.at(i).toElement().attribute("path")), 0);
        //qDebug() << "START " << dir.absoluteFilePath(fileList.at(i).toElement().attribute("path"));
        //fileReciever->setPId(m_ID);
        //emit recieverChanged(m_ID, fileReciever, i+1);
        fileReciever->run();
        delete fileReciever;

        emit progress(m_ID, i+1);
    }
    emit done(m_ID);    //This will be emitted even if canceled !!
    deleteLater();
}

void DirRecieverThread::stopTransfer()
{
    cancel = true;
}

DirRecieverThread::~DirRecieverThread()
{
    qDebug() << "DirRecieverThread END";
}
